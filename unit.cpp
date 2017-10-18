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
