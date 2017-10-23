#include "stdafx.h"
#include "unit.h"
#include "objectFactory.h"
#include "mncObjectBase.h"
#include "country.h"


void unitBuildTown::enter(unit& me)
{
	me._state = UnitState::BuildTown;
	_frameTimer = 0;
	//������ ������ ���� �� ����.
	objectFactory factory;
	WORLD->addObject(factory.createObject(_destIndex.x, _destIndex.y, me.getColorString() + "Town"));
	_obj = (mncObjectBase*)WORLD->getMap()->getTile(_destIndex.x, _destIndex.y)->getObjectOnTile();
	
	vector2D desttileindex(_destIndex);
	vector2D direction = desttileindex - me._index;

	if (direction.x == 1)
		me._unitDirection = UnitDirection::UNIT_RIGHT;
	else if (direction.x == -1)
		me._unitDirection = UnitDirection::UNIT_LEFT;
	else if (direction.y == 1)
		me._unitDirection = UnitDirection::UNIT_DOWN;
	else if (direction.y == -1)
		me._unitDirection = UnitDirection::UNIT_UP;
	else
		cout << direction.x << " , " << direction.y << "unitBuild enter ���⼳�� ����" << endl;


	_obj->setHp(1);
}

void unitBuildTown::update(unit& me)
{
	_frameTimer++;		
	
	int health = me.getHealth();
	int objHp = _obj->getHp();

	//���� �ִ��� üũ
	int cost = _obj->getCost() / _obj->getMaxHp(); //hp+1�� �ʿ� ���
	country* cty = WORLD->getCountry(me._unitColor);
	if (cty->getGold() >= cost)
	{
		cty->addGold(-cost);

		objHp += 1;
		_obj->setHp(objHp);
		health -= 1;
		me.setHp(health);
	}
	//���� ������ �Ǽ� �ߴ�
	else
	{
		return me.changeState(new unitNoneState);
	}

	//�Ǽ� �Ϸ�
	if (objHp >= _obj->getMaxHp())
	{
		_obj->setHp(_obj->getMaxHp());
		return me.changeState(new unitNoneState);
	}
	//�Ǽ� �� ������Ʈ�� �ı����� ��� �Ǽ� ���.
	if (objHp <= 0 || !_obj->isLive())
	{
		return me.changeState(new unitNoneState);
	}
	
}

bool unit::isBuildableTown(POINT index)
{
	//�ڱ� ü���� 50���� ���ƾ� ���� �� ����.
	if (_hp <= 50) return false;

	//index �����÷ο�, ���� üũ, �� ������ �ִ��� üũ	
	if (!isMoveable(index)) return false;
		
	//�Ǽ� �������� üũ
	terrainTile* tile = WORLD->getMap()->getTile(index.x, index.y);
	if (!tile->isBuildable()) return false;
	
	//�Ǽ��Ϸ��� �� �ֺ��� 8Ÿ���� �����ͼ� �Ʊ� ������ �ִ��� üũ
	//�Ʊ� ������ �̹� �Ǽ��Ǿ��ִٸ� �Ǽ� ����.
	terrainTile* tiles[8];
	WORLD->getMap()->get8Tiles(tiles, index.x, index.y);
	for (int i = 0; i < 8; ++i)
	{
		if (tiles[i])
		{
			mncObjectBase* objectOnTile = (mncObjectBase*)(tiles[i]->getObjectOnTile());
			if (objectOnTile != NULL)
			{
				if (objectOnTile->isLive())
				{
					if (objectOnTile->_name == "����" && objectOnTile->getCountryColor() == _unitColor)
					{
						return false;
						break;
					}
				}
			}
		}
	}

	return true;
}









//===================================================
// ## ������Ʈ �Ǽ�
void unitBuildObject::enter(unit& me)
{
	me._state = UnitState::BuildTown;
	_frameTimer = 0;

	vector2D destIndex = _destTile->getIndex();
	objectFactory factory;
	WORLD->addObject(factory.createObject(destIndex.x, destIndex.y, me.getColorString() + _key));
	_obj = (mncObjectBase*)WORLD->getMap()->getTile(destIndex.x, destIndex.y)->getObjectOnTile();

	//���� ������Ʈ�� �������� �ʾ����� �Ǽ��� ���.
	if (!_obj)
	{
		me.resetCommand();
		me.changeState(new unitNoneState);
		return;
	}

	vector2D direction = destIndex - me._index;

	if (direction.x == 1)
		me._unitDirection = UnitDirection::UNIT_RIGHT;
	else if (direction.x == -1)
		me._unitDirection = UnitDirection::UNIT_LEFT;
	else if (direction.y == 1)
		me._unitDirection = UnitDirection::UNIT_DOWN;
	else if (direction.y == -1)
		me._unitDirection = UnitDirection::UNIT_UP;
	else
		cout << direction.x << " , " << direction.y << "unitBuild enter ���⼳�� ����" << endl;

	_obj->setHp(1);
}

void unitBuildObject::update(unit& me)
{
	_frameTimer++;

	int health = me.getHealth();
	int objHp = _obj->getHp();

	if (_frameTimer % 2 == 0)
	{
		//���� �ִ��� üũ
		int cost = _obj->getCost() / _obj->getMaxHp(); //hp+1�� �ʿ� ���
		country* cty = WORLD->getCountry(me._unitColor);
		if (cty->getGold() >= cost)
		{
			cty->addGold(-cost);

			objHp += 1;
			_obj->setHp(objHp);
			health -= 1;
			me.setHp(health);
		}
		else
		{
			me._state = UnitState::NoMoney;
		}
		_frameTimer = 0;
	}

	//�Ǽ� �Ϸ�
	if (objHp >= _obj->getMaxHp())
	{
		_obj->setHp(_obj->getMaxHp());
		return me.changeState(new unitNoneState);
	}
	//�Ǽ� �� ������Ʈ�� �ı����� ��� �Ǽ� ���.
	if (objHp <= 0 || !_obj->isLive())
	{
		return me.changeState(new unitNoneState);
	}
}