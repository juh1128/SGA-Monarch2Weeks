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

		//못가는타일이면
		if (!me.isMoveable(destIndex))
		{
			//못가
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

			//갈수없는 타일이면
			if (!me.isMoveable(destIndex))
			{
				//못가
				isCanGo = false;
			}
		}

		//x와y다 돌았는데도 못가면 걍 여기서 갈수있는타일찾자
		if (!isCanGo)
		{
			vector2D index;
			for (int i = 0; i < 4; i++)
			{	
				index = me._index + me.getDirectionVector(UnitDirection::DIRECTION(i));
				if (me.isMoveable(index.toPoint()))
				{
					break;
				}
			}
			me.changeState(new unitOneStep(index.x,index.y));
		}
		else me.changeState(new unitOneStep(destIndex.x, destIndex.y));
	}

	//x거리가 더 짧다면
	if (distance.x < distance.y)
	{
		bool isCanGo = true;
		//내가 왼쪽에있다면
		if (me._index.x < _avoidUnit->_index.x)
		{
			destIndex.x -= 1;
		}
		else if (me._index.x > _avoidUnit->_index.x)
		{
			destIndex.x += 1;
		}

		//갈수있다면
		if (me.isMoveable(destIndex))
		{
			cout << "좌우이동으로 도망" << endl;
			me.changeState(new unitOneStep(destIndex.x,destIndex.y));
		}
		//갈수없다면
		else
		{
			vector2D index;
			for (int i = 0; i < 4; i++)
			{
				index = me.getDirectionVector(UnitDirection::DIRECTION(i));
				if (me.isMoveable(index.toPoint()))
				{
					return;
				}
			}
			cout << "좌우이동으로 도망" << endl;
			me.changeState(new unitOneStep(index.x, index.y));
		}
	}

	//y의 거리가 더 짧다면
	if (distance.x > distance.y)
	{
		bool isCanGo = true;
		//내가 위쪽에있다면
		if (me._index.y < _avoidUnit->_index.y)
		{
			destIndex.y -= 1;
		}
		else if (me._index.y > _avoidUnit->_index.y)
		{
			destIndex.y += 1;
		}

		//갈수있다면
		if (me.isMoveable(destIndex))
		{
			cout << "상하이동으로 도망" << endl;
			me.changeState(new unitOneStep(destIndex.x, destIndex.y));
		}
		//갈수없다면
		else
		{
			vector2D index;
			for (int i = 0; i < 4; i++)
			{
				index = me.getDirectionVector(UnitDirection::DIRECTION(i));
				if (me.isMoveable(index.toPoint()))
				{
					return;
				}
			}
			cout << "상하이동으로 도망" << endl;
			me.changeState(new unitOneStep(index.x, index.y));
		}
	}
}