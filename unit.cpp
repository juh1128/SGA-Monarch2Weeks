#include "stdafx.h"
#include "unit.h"


unit::unit()
{
}


unit::~unit()
{
}

HRESULT unit::init(vector2D index, int height,CountryColor::Enum country)
{
	//������ �Ҽӻ���
	_unitColor = country;

	string color;
	//����˻�
	switch (country)
	{
	case CountryColor::RED:
		color = "red";
		break;
	case CountryColor::WHITE:
		color = "white";
		break;
	case CountryColor::BLUE:
		color = "blue";
		break;
	case CountryColor::GREEN:
		color = "green";
		break;
	case CountryColor::END:
		break;
	default:
		break;
	}

	gameObject::init("unit", color + "Lv1", tileMap::getTilePosFromIndex(index, height), Pivot::CENTER);

	_pos = _pos / CAMERA->getZoom();
	_index = index;
	_height = height;
	_hp = 100;
	_isStarUnit = false;

	WORLD->getMap()->getTile(_index.x, _index.y)->addUnitOnTile(this);

	_imageFrameX = 0;
	_unitDirection = (UnitDirection::DIRECTION)0;

	_isAuto = true;
	_unitState = new unitNoneState;
	_moveSpeed = 1.5f;
	_frameTimer = 0;
	
	changeState(new unitCreateMotion);

	return S_OK;
}

void unit::release()
{
}

void unit::update()
{
	gameObject::update();

	imageFrame();

	auto keyboardTest = [&](UnitDirection::DIRECTION dir)
	{
		vector2D right = getDirectionVector(dir);
		vector2D dest = _index + right;

		gameObject* temp = WORLD->getMap()->getTile(dest.x, dest.y);

		vector<gameObject*> vr;
		vr.push_back(temp);

		this->sendMessage("move", 0, 0, 0, dest.toPoint(), vr);
		vr.clear();
	};

	if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
	{
		keyboardTest(UnitDirection::UNIT_RIGHT);
	}
	if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
	{
		keyboardTest(UnitDirection::UNIT_LEFT);
	}
	if (KEYMANAGER->isOnceKeyDown(VK_UP))
	{
		keyboardTest(UnitDirection::UNIT_UP);
	}
	if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
	{
		keyboardTest(UnitDirection::UNIT_DOWN);
	}

	_unitState->update(*this);

	//pos <-> �ε��� ����ȭ
	syncIndexFromPos();

	//Z���� �� ������ Ÿ�Ͽ��� �������� ��û�Ѵ�.
	requestRender();
}

void unit::render()
{
	_scale = vector2D(CAMERA->getZoom(), CAMERA->getZoom());

	if (_image)
	{
		_image->setAlphaOption(_alpha);
		_image->setScaleOption(_scale);
		_image->frameRender(_pos.x*CAMERA->getZoom(), _pos.y*CAMERA->getZoom(), _imageFrameX, _unitDirection, _pivot);
	}
}

void unit::changeState(unitState* newstate)
{
	SAFE_DELETE(_unitState);
	_unitState = newstate;
	_unitState->enter(*this);
}

vector2D unit::getDirectionVector(UnitDirection::DIRECTION dir)
{
	vector2D direction;

	switch (dir)
	{
	case UnitDirection::UNIT_LEFT: direction = vector2D(-1, 0);
		break;
	case UnitDirection::UNIT_UP: direction = vector2D(0, -1);
		break;
	case UnitDirection::UNIT_RIGHT:	direction = vector2D(1, 0);
		break;
	case UnitDirection::UNIT_DOWN: direction = vector2D(0, 1);
		break;
	}
	return direction;
}

void unit::syncIndexFromPos()
{
	float zoom = CAMERA->getZoom();
	_index = tileMap::getTileIndexFromPos(vector2D((_pos.x+0.1f)*zoom, (_pos.y+0.1f)*zoom), _height);
}

void unit::requestRender()
{
	//������ ������ ��� �ٸ� ���� �ʿ����
	if (_state != UnitState::MoveOneStep)
	{
		WORLD->getMap()->getTile(_index.x, _index.y)->requestRender(this);
		return;
	}
	
	//�̵� ���� ��� ĳ���� �̹��� LEFT, TOP, RIGHT, BOTTOM �κ� �˻� ��
	//���� ������ Ÿ�Ͽ� �������� ��û�Ѵ�.
	float zoom = CAMERA->getZoom();
	vector2D zoomedPos = _pos * zoom;
	RECT rc = RectMakeCenter(zoomedPos.x, zoomedPos.y, _size.x*zoom, _size.y*zoom);

	vector2D top = vector2D(zoomedPos.x, (float)rc.top);
	vector2D right = vector2D((float)rc.right, zoomedPos.y);
	vector2D bottom = vector2D(zoomedPos.x, (float)rc.bottom);
	vector2D left = vector2D((float)rc.left, zoomedPos.y);

	vector2D indexList[4];
	indexList[0] = tileMap::getTileIndexFromPos(top, _height);
	indexList[1] = tileMap::getTileIndexFromPos(right, _height);
	indexList[2] = tileMap::getTileIndexFromPos(bottom, _height);
	indexList[3] = tileMap::getTileIndexFromPos(left, _height);

	vector2D maxIndex = indexList[0];
	for (int i = 1; i < 4; ++i)
	{
		if (maxIndex.x <= indexList[i].x && maxIndex.y <= indexList[i].y)
		{
			maxIndex = indexList[i];
		}
	}

	WORLD->getMap()->getTile(maxIndex.x, maxIndex.y)->requestRender(this);
}

bool unit::isMoveable(POINT index)
{
	//�������� �� �� ���� ���� ��� NoneState�� ����.
	vector2D tileCount = WORLD->getMap()->getTileCount();

	// - �����÷ο� üũ
	if (index.x < 0 || index.x >= tileCount.x || index.y < 0 || index.y >= tileCount.y)
	{
		return false;
	}

	// - ����, �̵��Ұ��� Ÿ�� üũ
	terrainTile* destTile = WORLD->getMap()->getTile(index.x, index.y);
	int distanceHeight = abs(destTile->getHeight(true) - _height);
	if (!destTile->isWalkable() || distanceHeight > 1)
	{
		return false;
	}

	return true;
}

void unit::build()
{
	//�Ǽ��� ����ϴ� �Լ�





}


void unit::attack()
{
	//������ ����ϴ� �Լ�
}

void unit::run(unit* starUnit)
{
	//�������� �Լ�
	cout << "run" << endl;
	terrainTile* destTile;

	//x��y�Ÿ�
	int xDistance = UTIL::getDistance(_pos.x, 0, starUnit->_pos.x, 0);
	int yDistance = UTIL::getDistance(0, _pos.y, 0, starUnit->_pos.y);

	//������ X�Ÿ� Y�Ÿ��� ���Ѵ�
	if (abs(xDistance) == abs(yDistance))
	{
		//���� �����ʿ��ִٸ�
		if (_pos.x > starUnit->_pos.x)
		{
			destTile = WORLD->getMap()->getTile(_index.x + 1, _index.y);
		}
		//���� ���ʿ��ִٸ�
		else if (_pos.x < starUnit->_pos.x)
		{
			destTile = WORLD->getMap()->getTile(_index.x - 1, _index.y);
		}

		//�������ٸ�
		if (!destTile->isWalkable() || destTile->getIndex().x < 0 || destTile->getIndex().y
			|| destTile->getIndex().x > WORLD->getMap()->getTileCount().x
			|| destTile->getIndex().y > WORLD->getMap()->getTileCount().y)
		{
			//���� �Ʒ����ִٸ�
			if (_pos.y > starUnit->_pos.y)
			{
				destTile = WORLD->getMap()->getTile(_index.x, _index.y + 1);
			}
			//���� �����ִٸ�
			else if (_pos.y < starUnit->_pos.y - 1)
			{
				destTile = WORLD->getMap()->getTile(_index.x, _index.y - 1);
			}
			// �������ٸ�
				if (!destTile->isWalkable() || destTile->getIndex().x < 0 || destTile->getIndex().y
					|| destTile->getIndex().x > WORLD->getMap()->getTileCount().x
					|| destTile->getIndex().y > WORLD->getMap()->getTileCount().y
					|| destTile == NULL)
				{
					destTile = WORLD->getMap()->getTile(_index.x + getDirectionVector(_unitDirection).x, _index.y + getDirectionVector(_unitDirection).y);
				}
		}
	}

	//x�Ÿ��� �� ª�ٸ�
	if (abs(xDistance) < abs(yDistance))
	{
		//���� �����ʿ��ִٸ�
		if (_pos.x > starUnit->_pos.x)
		{
			destTile = WORLD->getMap()->getTile(_index.x + 1, _index.y);
		}
		//���� ���ʿ��ִٸ�
		else if (_pos.x < starUnit->_pos.x)
		{
			destTile = WORLD->getMap()->getTile(_index.x - 1, _index.y);
		}
		else if (_pos.x == starUnit->_pos.x)
		{
			xDistance = 0;
			yDistance = 1;
		}

		//�������ٸ�
		if (!destTile->isWalkable() || destTile->getIndex().x < 0 || destTile->getIndex().y
			|| destTile->getIndex().x > WORLD->getMap()->getTileCount().x
			|| destTile->getIndex().y > WORLD->getMap()->getTileCount().y
			|| destTile == NULL)
		{
			//x�� ���������� y�� �̵��ϱ�����
			xDistance = 0;
			yDistance = 1;
		}
	}

	//y�� �Ÿ��� �� ª�ٸ�
	if (abs(xDistance) > abs(yDistance))
	{
		//���� �Ʒ����ִٸ�
		if (_pos.y > starUnit->_pos.y)
		{
			destTile = WORLD->getMap()->getTile(_index.x, _index.y + 1);
		}
		//���� �����ִٸ�
		else if (_pos.y < starUnit->_pos.y - 1)
		{
			destTile = WORLD->getMap()->getTile(_index.x, _index.y - 1);
		}

		//�������ٸ�
		if (!destTile->isWalkable() || destTile->getIndex().x < 0 || destTile->getIndex().y
			|| destTile->getIndex().x > WORLD->getMap()->getTileCount().x
			|| destTile->getIndex().y > WORLD->getMap()->getTileCount().y
			|| destTile == NULL)
		{
			//x�� ���������� y�� �̵��ϱ�����
			xDistance = 1;
			yDistance = 0;
		}
	}

	//�ѹ��� x�� �˻�(y�˻翡�� �ٲ���������ֱ⶧��)
	if (abs(xDistance) < abs(yDistance))
	{
		//���� �����ʿ��ִٸ�
		if (_pos.x > starUnit->_pos.x)
		{
			destTile = WORLD->getMap()->getTile(_index.x + 1, _index.y);
		}
		//���� ���ʿ��ִٸ�
		else if (_pos.x < starUnit->_pos.x)
		{
			destTile = WORLD->getMap()->getTile(_index.x - 1, _index.y);
		}
		else if (_pos.x == starUnit->_pos.x)
		{
			//���� �Ʒ����ִٸ�
			if (_pos.y > starUnit->_pos.y)
			{
				destTile = WORLD->getMap()->getTile(_index.x, _index.y + 1);
			}
			//���� �����ִٸ�
			else if (_pos.y < starUnit->_pos.y - 1)
			{
				destTile = WORLD->getMap()->getTile(_index.x, _index.y - 1);
			}
		}

		//�������ٸ�
		if (!destTile->isWalkable() || destTile->getIndex().x < 0 || destTile->getIndex().y
			|| destTile->getIndex().x > WORLD->getMap()->getTileCount().x
			|| destTile->getIndex().y > WORLD->getMap()->getTileCount().y
			|| destTile == NULL)
		{
			//���⼭�� x�� y�� ��� �����Ѱű⶧���� �׳� ������ �����Ѵ�
			//�׳� ������ΰ���
			destTile = WORLD->getMap()->getTile(_index.x + getDirectionVector(_unitDirection).x, _index.y + getDirectionVector(_unitDirection).y);
		}
	}

	POINT directionIndex;
	directionIndex.x = destTile->getIndex().x;
	directionIndex.y = destTile->getIndex().y;

	if (destTile == NULL) return;
		changeState(new unitOneStep(directionIndex.x, directionIndex.y));
}

void unitNoneState::enter(unit & me)
{
	me._state = UnitState::None;
	if (me._isAuto)
	{
		//���⿡ ���� Ÿ�� ��ĭ����
		terrainTile* tile[2];
		vector2D direction = me.getDirectionVector(me._unitDirection);
		vector2D temp = me._index + direction;
		vector2D temp2 = me._index + direction * 2;

		tile[0] = WORLD->getMap()->getTile(temp.x, temp.y);
		tile[1] = WORLD->getMap()->getTile(temp2.x, temp2.y);

		//���� ����->�Ǽ�->����

		//�������� ��Ȳ
		//�ڽ����κ��� 25ĭ(�����¿�밢���� 2ĭ��)
		//�ֺ��� �θ����̻��� ������� ���� ü���� �����ַκ��� ������

		//����
		judgeRun(me);

		//�ǹ����� ��Ȳ
		//judgeBuild(me);
	}
}


void unitNoneState::update(unit & me)
{
	moveOneStep(me);
}


void unitNoneState::moveOneStep(unit& me)
{
	//�ڱⰡ �ٶ󺸰� �ִ� �������� �̵��Ѵ�.
	vector2D destIndex = me._index + me.getDirectionVector(me._unitDirection);

	//1. 4���� �˻� �� �̵� ������ ������ �迭�� ��´�.
	vector<vector2D> directionCheck;
	for (int i = 0; i < 4; ++i)
	{
		vector2D dIndex = me._index + me.getDirectionVector((UnitDirection::DIRECTION)i);
		if (me.isMoveable(dIndex.toPoint()))
		{
			directionCheck.push_back(dIndex);
		}
	}

	//2. �̵� ������ ���� �� destIndex�� ������ �� �������� �̵�.
	//������ ���� �������� �̵�.
	size_t size = directionCheck.size();
	bool isRndDirection = true;
	for (size_t i = 0; i < size; ++i)
	{
		if (directionCheck[i] == destIndex)
		{
			me.changeState(new unitOneStep(destIndex.x, destIndex.y));
			isRndDirection = false;
			break;
		}
	}
	if (size > 0 && isRndDirection)
	{
		vector2D rndDirection = directionCheck[RND->getFromIntTo(0, size - 1)];
		me.changeState(new unitOneStep(rndDirection.x, rndDirection.y));
	}
}

void unitNoneState::judgeRun(unit& me)
{
	//ã�Ƴ� ���ֵ��� ���� ����
	vector<unit*> searchedUnit;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			//����ó��
			if (j == 2 && i == 2) continue;
			if (j > 0 && j < 4) continue;
			if (j + me._index.x - 2 < 0 || j + me._index.x - 2 > WORLD->getMap()->getTileCount().x) continue;
			if (i + me._index.y - 2 < 0 || i + me._index.y - 2 > WORLD->getMap()->getTileCount().y - 1) continue;

			//�ش� Ÿ�Ͽ� �ִ� ���ֺ���
			vector<unit*> unitOnTile = WORLD->getMap()->getTile(j - 2 + me._index.x, i - 2 + me._index.y)->getUnitOnTile();


			//Ÿ�Ͽ� ������ �ִٸ�
			if (unitOnTile.size() != NULL)
			{
				//Ÿ�Ͽ��ִ� ���ֺ��͸� ���� ������ ü���� ���ٸ� ����
				for (int k = 0; k <unitOnTile.size(); k++)
				{
					//�� �ڽŰ� ������ ���ٸ� ������� �ʴ´�.
					if (unitOnTile[k]->getCountryColor() == me._unitColor) continue;

					//����ü�� + 200 �� Ÿ������ ������ ü�� ���� �������
					if (me._hp + 200 < unitOnTile[k]->getHealth())
					{
						searchedUnit.push_back(unitOnTile[k]);
					}
				}
			}
		}
	}

	//����Ž���Ͽ� ���Ϳ� �� ������Ƿ� ������� �ʴٸ� ������ ã�����̴�.
	//�׷��� �������� �Լ��� �����Ѵ�.
	if (searchedUnit.size() != NULL)
	{
		//ü���� ���� ���� ������ ã�Ƴ���
		int maximumHealth = 0;
		int maximumUnitIndex = 0;
		for (int k = 0; k < searchedUnit.size(); k++)
		{
			//ã�������� �̹������� �ִ�ü�º��� ���ٸ�
			if (searchedUnit[k]->getHealth() > maximumHealth)
			{
				//�ִ�ü�°� �ִ�ü���� ���� ������ �ε����� ����
				maximumHealth = searchedUnit[k]->getHealth();
				maximumUnitIndex = k;
			}
		}

		me.run(searchedUnit[maximumUnitIndex]);

		return;
	}
}


void unitOneStep::enter(unit & unit)
{
	// - �̵��Ÿ� üũ (�ε��� ���̰� 2ĭ �̻� �� ��� �̵����� ����)
	vector2D distance = _destIndex - unit._index;
	if (distance.getLength() > 1)
	{
		unit.changeState(new unitNoneState);
	}

	unit._state = UnitState::MoveOneStep;
	_oldIndex = unit._index;
	_destTile = WORLD->getMap()->getTile(_destIndex.x, _destIndex.y);

	//���� ���� ����
	if (distance.x == 1)
		unit._unitDirection = UnitDirection::UNIT_RIGHT;
	else if (distance.x == -1)
		unit._unitDirection = UnitDirection::UNIT_LEFT;
	else if (distance.y == 1)
		unit._unitDirection = UnitDirection::UNIT_DOWN;
	else if (distance.y == -1)
		unit._unitDirection = UnitDirection::UNIT_UP;
	else
		cout << "���� ���� ���� ����" << endl;

}

void unitOneStep::update(unit & unit)
{
	float zoom = CAMERA->getZoom();
	vector2D destPos = tileMap::getTilePosFromIndex(_destIndex, _destTile->getHeight(true)) / zoom;
	vector2D distance = destPos - unit._pos;

	//������ ����
	float length = distance.getLength();
	if (length <= unit._moveSpeed)
	{
		//������ ���ִ� Ÿ�� ����
		WORLD->getMap()->getTile(_oldIndex.x, _oldIndex.y)->deleteUnitOnTile(&unit);
		WORLD->getMap()->getTile(_destIndex.x, _destIndex.y)->addUnitOnTile(&unit);
		unit._pos = destPos;
		unit.changeState(new unitNoneState);
	}
	//���� ��
	else
	{
		unit._pos = unit._pos + distance.normalize()*unit._moveSpeed;

		//4�� ���� ������ Ÿ�� �������� ��Ÿ�� ���� �̵��� �����̱� ���� => Ÿ�ϻ�������� ������ ������ �̵��� �Ÿ��̴�
		if (length <= tileMap::getTileSize().getLength()*zoom*0.25f)
		{
			unit._height = _destTile->getHeight(true);
		}
	}

}

