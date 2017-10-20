#include "stdafx.h"
#include "unit.h"
#include "objectFactory.h"
#include "mncObjectBase.h"


void unitBuildTown::enter(unit& me)
{
	me._state = UnitState::BuildTown;
	_frameTimer = 0;
	//마을이 없으면 지을 수 있음.
	objectFactory factory;
	WORLD->addObject(factory.createObject(_destIndex.x, _destIndex.y, me.getColorString() + "Town"));
	_obj = (mncObjectBase*)WORLD->getMap()->getTile(_destIndex.x, _destIndex.y)->getObjectOnTile();
	_obj->setHp(1);
}

void unitBuildTown::update(unit& me)
{
	_frameTimer++;		
	
	int health = me.getHealth();
	int objHp = _obj->getHp();

	if (_frameTimer % 2 == 0)
	{
		objHp += 1;
		_obj->setHp(objHp);
		health -= 1;
		me.setHp(health);

		_frameTimer = 0;
	}	

	if (objHp >= 50)
	{
		cout << "잡둰" << endl;
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