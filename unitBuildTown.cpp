#include "stdafx.h"
#include "unit.h"
#include "objectFactory.h"
#include "mncObjectBase.h"
#include "country.h"


void unitBuildTown::enter(unit& me)
{
	me._state = UnitState::BuildTown;
	_frameTimer = 0;
	//마을이 없으면 지을 수 있음.
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
		cout << direction.x << " , " << direction.y << "unitBuild enter 방향설정 오류" << endl;


	_obj->setHp(1);
}

void unitBuildTown::update(unit& me)
{
	_frameTimer++;		
	
	int health = me.getHealth();
	int objHp = _obj->getHp();

	//돈이 있는지 체크
	int cost = _obj->getCost() / _obj->getMaxHp(); //hp+1당 필요 골드
	country* cty = WORLD->getCountry(me._unitColor);
	if (cty->getGold() >= cost)
	{
		cty->addGold(-cost);

		objHp += 1;
		_obj->setHp(objHp);
		health -= 1;
		me.setHp(health);
	}
	//돈이 없으면 건설 중단
	else
	{
		return me.changeState(new unitNoneState);
	}

	//건설 완료
	if (objHp >= _obj->getMaxHp())
	{
		_obj->setHp(_obj->getMaxHp());
		return me.changeState(new unitNoneState);
	}
	//건설 중 오브젝트가 파괴됬을 경우 건설 취소.
	if (objHp <= 0 || !_obj->isLive())
	{
		return me.changeState(new unitNoneState);
	}
	
}

bool unit::isBuildableTown(POINT index)
{
	//자기 체력이 50보다 많아야 지을 수 있음.
	if (_hp <= 50) return false;

	//index 오버플로우, 높이 체크, 적 유닛이 있는지 체크	
	if (!isMoveable(index)) return false;
		
	//건설 가능한지 체크
	terrainTile* tile = WORLD->getMap()->getTile(index.x, index.y);
	if (!tile->isBuildable()) return false;
	
	//건설하려는 곳 주변의 8타일을 가져와서 아군 마을이 있는지 체크
	//아군 마을이 이미 건설되어있다면 건설 못함.
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
					if (objectOnTile->_name == "마을" && objectOnTile->getCountryColor() == _unitColor)
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
// ## 오브젝트 건설
void unitBuildObject::enter(unit& me)
{
	me._state = UnitState::BuildTown;
	_frameTimer = 0;

	vector2D destIndex = _destTile->getIndex();
	objectFactory factory;
	WORLD->addObject(factory.createObject(destIndex.x, destIndex.y, me.getColorString() + _key));
	_obj = (mncObjectBase*)WORLD->getMap()->getTile(destIndex.x, destIndex.y)->getObjectOnTile();

	//만약 오브젝트가 생성되지 않았으면 건설을 취소.
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
		cout << direction.x << " , " << direction.y << "unitBuild enter 방향설정 오류" << endl;

	_obj->setHp(1);
}

void unitBuildObject::update(unit& me)
{
	_frameTimer++;

	int health = me.getHealth();
	int objHp = _obj->getHp();

	if (_frameTimer % 2 == 0)
	{
		//돈이 있는지 체크
		int cost = _obj->getCost() / _obj->getMaxHp(); //hp+1당 필요 골드
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

	//건설 완료
	if (objHp >= _obj->getMaxHp())
	{
		_obj->setHp(_obj->getMaxHp());
		return me.changeState(new unitNoneState);
	}
	//건설 중 오브젝트가 파괴됬을 경우 건설 취소.
	if (objHp <= 0 || !_obj->isLive())
	{
		return me.changeState(new unitNoneState);
	}
}