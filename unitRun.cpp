#include "stdafx.h"
#include "unit.h"

void unitRun::enter(unit& me)
{
	//도망가는 함수
	cout << "run" << endl;
	terrainTile* destTile;
	POINT destIndex;
	destIndex.x = me._index.x;
	destIndex.y = me._index.y;

	//x와y거리
	POINT distance;
	distance.x = UTIL::getDistance(me._index.x, 0, _avoidUnit->_index.x, 0);
	distance.y = UTIL::getDistance(0, me._index.y, 0, _avoidUnit->_index.y);

	//if (me.isMoveable(destIndex))
	//{
	//
	//}
	//else
	//{
	//
	//}

	//서로의 X거리 Y거리가 같다면
	if (distance.x == distance.y)
	{
		bool isCanGo = true;
		//둘의 길이가 같다면 x축으로 먼저 이동
		//내 유닛이 왼쪽에있다면 왼쪽이동해야함
		if (me._index.x <= _avoidUnit->_index.x)
		{
			destIndex.x -= 1;
		}
		//내 유닛이 오른쪽에 있다면 오른쪽으로 이동해야함
		else if (me._index.x > _avoidUnit->_index.x)
		{
			destIndex.x += 1;
		}

		if (WORLD->getMap()->getTile(destIndex.x, destIndex.y)->isWalkable())
		{
			if (destIndex.x < 0 || destIndex.y <0
				|| destIndex.y > WORLD->getMap()->getTileCount().y
				|| destIndex.x > WORLD->getMap()->getTileCount().x)
			{
				isCanGo = false;
			}
		}
		else
		{
			isCanGo = false;
		}

		//가려했던 타일이 못간다면 y축으로 이동해야한다
		if (!isCanGo)
		{
			isCanGo = true;
			destIndex.x = me._index.x;
			destIndex.y = me._index.y;
			//내 유닛이 위에있다면
			if (me._index.y <= _avoidUnit->_index.y)
			{
				destIndex.y -= 1;
			}
			else if (me._index.y > _avoidUnit->_index.y)
			{
				destIndex.y += 1;
			}

			//갈수있는타일인데
			if (WORLD->getMap()->getTile(destIndex.x, destIndex.y)->isWalkable())
			{
		
			}
			//갈수없는타일이면
			else
			{
				//못가
				isCanGo = false;
			}
		}

		//x와y다 돌았는데도 못가면 걍 원스텝으로 보내자 알아서 하겠지 뭐
		if (!isCanGo)
		{
			for (int i = 0; i < 4; i++)
			{	
				//갈수있으면
				if (WORLD->getMap()->getTile(me._index.x + me.getDirectionVector((UnitDirection::DIRECTION)i).x, me._index.y + me.getDirectionVector((UnitDirection::DIRECTION)i).y))
				{

				}
			}
		}
	}

	//x거리가 더 짧다면
	if (distance.x < distance.y)
	{
		
	}

	//y의 거리가 더 짧다면
	if (distance.x > distance.y)
	{
		
	}

	//POINT directionIndex;
	//directionIndex.x = destTile->getIndex().x;
	//directionIndex.y = destTile->getIndex().y;
	//
	//me.changeState(new unitOneStep(directionIndex.x, directionIndex.y));
}

