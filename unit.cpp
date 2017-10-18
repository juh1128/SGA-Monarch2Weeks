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

	this->addCallback("move", [&](tagMessage msg)
	{
		this->moveCallBack(msg.ptData, msg.targetList[0]);
	});

	_isAuto = true;
	_unitState = new unitNoneState;
	_moveSpeed = 3.0f;
	_livedTime = 0;
	
	changeState(new unitNoneState);

	return S_OK;
}

void unit::release()
{
}

void unit::update()
{
	gameObject::update();

	_livedTime += TIMEMANAGER->getElapsedTime();

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

void unit::moveCallBack(POINT directionTile, gameObject* dest)
{
	//if (directionTile.x < 0 ||
	//	directionTile.y < 0 ||
	//	directionTile.x >= WORLD->getMap()->getTileCount().x ||
	//	directionTile.y >= WORLD->getMap()->getTileCount().y	)
	//{
	//	return changeState(new unitNoneState);
	//}
	//changeState(new unitOneStep((terrainTile*)dest, directionTile));

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
	cout << "����" << endl;
	terrainTile* destTile;

	int xDistance = UTIL::getDistance(_pos.x, 0, starUnit->_pos.x, 0);
	int yDistance = UTIL::getDistance(0, _pos.y, 0, starUnit->_pos.y);

	//������ X�Ÿ� Y�Ÿ��� ���Ѵ�

	//x�Ÿ��� �� ��ٸ�
	if (abs(xDistance) > abs(yDistance))
	{
		//���� �����ʿ��ִٸ�
		if (_index.x > starUnit->_index.x)
		{

		}
		//���� ���ʿ��ִٸ�
		else if (_index.x < starUnit->_index.x)
		{

		}

		//�������ٸ�
		if (!destTile->isWalkable() || destTile->getIndex().x < 0 || destTile->getIndex().y
			|| destTile->getIndex().x > WORLD->getMap()->getTileCount().x
			|| destTile->getIndex().y > WORLD->getMap()->getTileCount().y)
		{
			//x�� ���������� y�� �̵��ϱ�����
			xDistance = 0;
			yDistance = 1;
			
		}
	}

	if (abs(xDistance) < abs(yDistance))
	{
		//���� �Ʒ����ִٸ�
		if (_index.y > starUnit->_index.y)
		{

		}
		//���� �����ִٸ�
		else if (_index.y < starUnit->_index.y)
		{

		}

		//�������ٸ�
		if (!destTile->isWalkable() || destTile->getIndex().x < 0 || destTile->getIndex().y
			|| destTile->getIndex().x > WORLD->getMap()->getTileCount().x
			|| destTile->getIndex().y > WORLD->getMap()->getTileCount().y)
		{
			//x�� ���������� y�� �̵��ϱ�����
			xDistance = 1;
			yDistance = 0;
		}
	}

	//�ѹ��� x�� �˻�(y�˻翡�� �ٲ���������ֱ⶧��)
	if (abs(xDistance) > abs(yDistance))
	{
		//���� �����ʿ��ִٸ�
		if (_index.x > starUnit->_index.x)
		{

		}
		//���� ���ʿ��ִٸ�
		else if (_index.x < starUnit->_index.x)
		{

		}

		//�������ٸ�
		if (!destTile->isWalkable() || destTile->getIndex().x < 0 || destTile->getIndex().y
			|| destTile->getIndex().x > WORLD->getMap()->getTileCount().x
			|| destTile->getIndex().y > WORLD->getMap()->getTileCount().y)
		{
			//�׳� ����ġ�� ��������� ������
		}
	}

	POINT directionIndex;
	directionIndex.x = destTile->getIndex().x;
	directionIndex.y = destTile->getIndex().y;


	//changeState(new unitOneStep(destTile, directionIndex));

	//this->sendMessage("move", 0, 0, 0, _index, tileVector);
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
		//���޸� ���ʹ̰� �ִٸ� �������� �Լ�����
		//�ֺ��� �θ����̻��� ������� ���� ü���� �����ַκ��� ������

		//ã�Ƴ� ���ֵ�
		//vector<unit*> searchedUnit;
		//for (int i = 0; i < 5; i++)
		//{
		//	for (int j = 0; j < 5; j++)
		//	{
		//		//����ó��
		//		if (j - 2 + me._index.x < 0 || j - 2 + me._index.x > WORLD->getMap()->getTileCount().x
		//			|| i - 2 + me._index.y < 0 || i - 2 + me._index.y > WORLD->getMap()->getTileCount().y
		//			|| (j== 2 && i == 2)) continue;

		//		//�ش� Ÿ�Ͽ� �ִ� ���ֺ���
		//		vector<unit*> unitOnTile = WORLD->getMap()->getTile(j - 2 + me._index.x, i - 2 + me._index.y)->getUnitOnTile();
		//		

		//		//Ÿ�Ͽ� ������ �ִٸ�
		//		if (WORLD->getMap()->getTile(j - 2 + me._index.x, i - 2 + me._index.y)->getUnitOnTile().size() != NULL)
		//		{
		//			//Ÿ�Ͽ��ִ� ���ֺ��͸� ���� ������ ü���� ���ٸ� ����
		//			for (int k = 0; k <unitOnTile.size();k++)
		//			{
		//				//�� �ڽŰ� ������ ���ٸ� ������� �ʴ´�.
		//				if (unitOnTile[k]->getCountryColor() == me._unitColor) continue;
		//				//�� ������ ���޸� ������ �ƴ϶��
		//				if (!unitOnTile[k]->getIsStarUnit()) continue;

		//				//����ü�� + 200 �� Ÿ������ ������ ü�� ���� �������
		//				if (me._hp + 200 < unitOnTile[k]->getHealth())
		//				{
		//					searchedUnit.push_back(unitOnTile[k]);
		//				}
		//			}
		//		}
		//	}
		//}

		////����Ž���Ͽ� ���Ϳ� �� ������Ƿ� ������� �ʴٸ� ������ ã�����̴�.
		////�׷��� �������� �Լ��� �����Ѵ�.
		//if (searchedUnit.size() != NULL)
		//{
		//	//ü���� ���� ���� ������ ã�Ƴ���
		//	int maximumHealth = 0;
		//	int maximumUnitIndex = 0;
		//	for (int k = 0; k < searchedUnit.size(); k++)
		//	{
		//		//ã�������� �̹������� �ִ�ü�º��� ���ٸ�
		//		if (searchedUnit[k]->getHealth() > maximumHealth)
		//		{
		//			//�ִ�ü�°� �ִ�ü���� ���� ������ �ε����� ����
		//			maximumHealth = searchedUnit[k]->getHealth();
		//			maximumUnitIndex = k;
		//		}
		//	}

		//	me.run(searchedUnit[maximumUnitIndex]);

		//	return;
		//}

		//�ǹ����� ��Ȳ
		//tile[0]���� �����¿�밢 ��ĭ���� Ž���Ͽ� �ǹ��� ���ٸ� �Ǽ��Լ� ����
		//for (int j = 0; j < 3; j++)
		//{
		//	for (int k = 0; k < 3; k++)
		//	{
		//		
		//
		//	}
		//}


		//tile�� �����Ͽ� ���̳� �ı������� ������Ʈ�� �ִٸ� �����Լ� ����
		//for (int i = 0; i < 2; i++)
		//{
		//
		//}





		
		
		

		


		//===========================================================================

		//	//POINT destp;
		//	//destp.x = dest.x;
		//	//destp.y = dest.y;

		//	if (tile[0] == nullptr)
		//	{
		//		while (true)
		//		{
		//			me._unitDirection = (UnitDirection::DIRECTION)RND->getFromIntTo(0, 3);
		//			cout << me._unitDirection << endl;
		//			direction = me.getDirectionVector(me._unitDirection);
		//			temp = me._index + direction;
		//			temp2 = me._index + direction * 2;

		//			tile[0] = WORLD->getMap()->getTile(temp.x, temp.y);
		//			tile[1] = WORLD->getMap()->getTile(temp2.x, temp2.y);

		//			if (tile[0] == nullptr) continue;

		//			dest = me._index + direction;

		//			destp.x = dest.x;
		//			destp.y = dest.y;

		//			if (dest.x >= 0 || dest.y >= 0 || dest.x < WORLD->getMap()->getTileCount().x || dest.y < WORLD->getMap()->getTileCount().y)
		//			{
		//				if (abs(tile[0]->getHeight() - WORLD->getMap()->getTile(me._index.x, me._index.y)->getHeight()) < 2)
		//				{
		//					if (WORLD->getMap()->getTile(destp.x, destp.y)->isWalkable())
		//					{
		//						break;
		//					}
		//				}
		//			}
		//		}

		//	}
		//	else
		//	{
		//		if (dest.x < 0 || dest.y < 0 || dest.x >= WORLD->getMap()->getTileCount().x || dest.y >= WORLD->getMap()->getTileCount().y)
		//		{
		//			if (abs(tile[0]->getHeight() - WORLD->getMap()->getTile(me._index.x, me._index.y)->getHeight()) >= 2)
		//			{
		//				if (!WORLD->getMap()->getTile(destp.x, destp.y)->isWalkable())
		//				{
		//					while (true)
		//					{
		//						me._unitDirection = (UnitDirection::DIRECTION)RND->getFromIntTo(0, 3);
		//						cout << me._unitDirection << endl;

		//						direction = me.getDirectionVector(me._unitDirection);
		//						temp = me._index + direction;
		//						temp2 = me._index + direction * 2;

		//						tile[0] = WORLD->getMap()->getTile(temp.x, temp.y);
		//						tile[1] = WORLD->getMap()->getTile(temp2.x, temp2.y);

		//						dest = me._index + direction;

		//						destp.x = dest.x;
		//						destp.y = dest.y;

		//						if (dest.x >= 0 || dest.y >= 0 || dest.x < WORLD->getMap()->getTileCount().x || dest.y < WORLD->getMap()->getTileCount().y)
		//						{
		//							if (abs(tile[0]->getHeight() - me._height) < 2)
		//							{
		//								if (WORLD->getMap()->getTile(destp.x, destp.y)->isWalkable())
		//								{
		//									break;
		//								}
		//							}
		//						}
		//					}
		//				}
		//			}
		//		}

		//	}
		//	gameObject* desttile = WORLD->getMap()->getTile(dest.x, dest.y);
		//	vector<gameObject*> vr;
		//	vr.push_back(desttile);
		//	me.sendMessage("move", 0, 0, 0, destp, vr);
		//	vr.clear();
		//}
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



//���� 1ĭ�� �̵�. 
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
