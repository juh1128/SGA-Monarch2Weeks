#include "stdafx.h"
#include "unit.h"
#include "objectFactory.h"


void unitBuildTown::enter(unit& me)
{
	me._state = UnitState::BuildTown;

	//마을이 없으면 지을 수 있음.
	objectFactory factory;
	WORLD->addObject(factory.createObject(_destIndex.x, _destIndex.y, me.getCountryColor() + "Town"));
	me._hp -= 30;

	me.changeState(new unitNoneState);
}

void unitBuildTown::update(unit& me)
{

}

bool unit::isBuildableTown(POINT index)
{
	//건설을 담당하는 함수
	cout << "마을건설" << endl;

	if (!isMoveable(index))
	{
		//terrainTile* tile = WORLD->getMap()->getTile(index.x, index.y);
		return false;
	}
	

	terrainTile* tiles[8];
	WORLD->getMap()->get8Tiles(tiles, index.x, index.y);

	//마을이 있는지 체크
	for (int i = 0; i < 8; ++i)
	{
	
		if (tiles[i])
		{
			//오브젝트가 없는 타일
			
			gameObject* objectOnTile = tiles[i]->getObjectOnTile();
			if (objectOnTile != NULL)
			{
				if (objectOnTile->_name == "마을")
				{
					return false;
					break;
				}

			}
		}
	}

	return true;
}