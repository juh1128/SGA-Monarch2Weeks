#include "stdafx.h"
#include "unit.h"


void unitNoneState::enter(unit & me)
{
	me._state = UnitState::None;
	if (me._isAuto)
	{
		//방향에 따른 타일 두칸검출
		terrainTile* tile[2];
		vector2D direction = me.getDirectionVector(me._unitDirection);
		vector2D temp = me._index + direction;
		vector2D temp2 = me._index + direction * 2;

		tile[0] = WORLD->getMap()->getTile(temp.x, temp.y);
		tile[1] = WORLD->getMap()->getTile(temp2.x, temp2.y);

		//순서 도망->건설->공격

		//도망가는 상황
		//자신으로부터 25칸(상하좌우대각으로 2칸씩)
		//주변에 두마리이상이 있을경우 가장 체력이 높은애로부터 도망감

	}
}


void unitNoneState::update(unit & me)
{
	moveOneStep(me);

	unit* dangerousUnit = me.isCanRun();
	if (dangerousUnit != NULL)
	{
		me.changeState(new unitRun(dangerousUnit));
	}
}


void unitNoneState::moveOneStep(unit& me)
{
	//자기가 바라보고 있는 방향으로 이동한다.
	vector2D destIndex = me._index + me.getDirectionVector(me._unitDirection);

	//1. 4방향 검사 후 이동 가능한 방향을 배열에 담는다.
	vector<vector2D> directionCheck;
	for (int i = 0; i < 4; ++i)
	{
		vector2D dIndex = me._index + me.getDirectionVector((UnitDirection::DIRECTION)i);
		if (me.isMoveable(dIndex.toPoint()))
		{
			directionCheck.push_back(dIndex);
		}
	}

	//2. 이동 가능한 방향 중 destIndex가 있으면 그 방향으로 이동.
	//없으면 랜덤 방향으로 이동.
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

