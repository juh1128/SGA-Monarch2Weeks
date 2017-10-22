#include "stdafx.h"
#include "unit.h"


void unitNoneState::enter(unit & me)
{
	//me._state = UnitState::None;
	//if (me._isAuto)
	//{
		//방향에 따른 타일 두칸검출
		//terrainTile* tile[2];
		//vector2D direction = me.getDirectionVector(me._unitDirection);
		//vector2D temp = me._index + direction;
		//vector2D temp2 = me._index + direction * 2;

		//tile[0] = WORLD->getMap()->getTile(temp.x, temp.y);
		//tile[1] = WORLD->getMap()->getTile(temp2.x, temp2.y);

		//순서 도망->건설->공격

		//도망가는 상황
		//자신으로부터 25칸(상하좌우대각으로 2칸씩)
		//주변에 두마리이상이 있을경우 가장 체력이 높은애로부터 도망감

	//}
}


void unitNoneState::update(unit & me)
{
	//플레이어로부터 받은 지시가 있을 경우
	// - 유닛 관련 명령
	if (me._commandTargetUnit)
	{		
		if (me._commandStateName == "추적")
		{
			vector2D distance = me._commandTargetUnit->_index - me._index;
			//해당 유닛의 1칸 앞에 왔을 때
			if (distance.getLength() <= 1)
			{
				me.changeState(new unitFight(me._commandTargetUnit));
				me.resetCommand();
				return;
			}
		}
		else if(me._commandStateName == "원군")
		{
			if (me._commandTargetUnit->_index == me._index)
			{
				me.resetCommand();
				return;
			}
		}
		
		//싸울 수 있으면 싸운다.
		unit* enemy = me.isCanAttack();
		if (enemy != NULL)
		{
			return me.changeState(new unitFight(enemy));
		}

		//길찾기 (유닛 추적)
		deque<terrainTile*> path = PATHFINDER->getPath(WORLD->getMap()->getTile(me._index.x, me._index.y), 
			WORLD->getMap()->getTile(me._commandTargetUnit->_index.x, me._commandTargetUnit->_index.y));
		if (path.size() > 0)
		{
			vector2D pathIndex = path[0]->getIndex();
			me.changeState(new unitOneStep(pathIndex.x, pathIndex.y));
		}
		else
		{
			cout << "길 못찾음" << endl;
			me.resetCommand();
		}
		return;
	}
	// - 타일 관련 명령
	//else if (me._commandDestTile)
	//{
	//	deque<terrainTile*> path = PATHFINDER->getPath(WORLD->getMap()->getTile(me._index.x, me._index.y),
	//		WORLD->getMap()->getTile(me._commandDestTile->getIndex().x, me._commandDestTile->getIndex().y));
	//	if (path.size() > 0)
	//	{
	//		vector2D pathIndex = path[0]->getIndex();
	//		return me.changeState(new unitOneStep(pathIndex.x, pathIndex.y));
	//	}
	//}


	//지시가 없으면 자동모드
	if (me._isAuto)
	{
		//도망
		//unit* dangerousUnit = me.isCanRun();
		//if (dangerousUnit != NULL)
		//{
		//	return me.changeState(new unitRun(dangerousUnit));

		//}

		//공격
		unit* enemy = me.isCanAttack();
		if (enemy != NULL)
		{
			return me.changeState(new unitFight(enemy));
		}

		//if (me._mergeUnit != nullptr)
		//{
		//	return me.changeState(new unitMerge(me._mergeUnit,me));

		//}
		me._state = UnitState::Search;

		mncObjectBase* nature = me.isCanAttackNature();
		if (nature != nullptr)
		{
			return me.changeState(new unitDigObject(nature));
		}
		//건설
		vector2D destIndex = me._index + me.getDirectionVector(me._unitDirection);
		if (me.isBuildableTown(destIndex.toPoint()))
		{
			return me.changeState(new unitBuildTown(destIndex.toPoint()));
		}

		//이동 (계속 이동만하고 딱히 행동을 할게 없으면 가장 가까운 유닛에게 병합한다.)
		moveOneStep(me);
	}
	else
	{
		me._state = UnitState::Stop;
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

