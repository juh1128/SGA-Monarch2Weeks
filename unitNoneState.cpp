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
	////예약 상태 확인
	//if (me._reservedState.size() > 0)
	//{
	//	unitState* reservedState = me._reservedState[0];
	//	//조건 체크
	//	bool check = false;
	//	if (me._reservedState[0]->_stateName == "전투")
	//	{
	//		//공격
	//		unit* enemy = me.isCanAttack();
	//		if (enemy != NULL)
	//		{
	//			check = true;
	//			//return me.changeState(new unitFight(enemy));
	//		}
	//	}
	//	else if (me._reservedState[0]->_stateName == "머지")
	//	{
	//		if (me._mergeUnit != nullptr)
	//		{
	//			check = true;
	//			//return me.changeState(new unitMerge(me._mergeUnit, me));
	//		}
	//	}
	//	else if (me._reservedState[0]->_stateName == "파괴")
	//	{
	//		mncObjectBase* nature = me.isCanAttackNature();
	//		if (nature != nullptr)
	//		{
	//			check = true;
	//			//return me.changeState(new unitDigObject(nature));
	//		}

	//	}
	//	//건설
	//	else if (me._reservedState[0]->_stateName == "건설")
	//	{
	//		vector2D destIndex = me._index + me.getDirectionVector(me._unitDirection);
	//		if (me.isBuildableTown(destIndex.toPoint()))
	//		{
	//			check = true;
	//			//return me.changeState(new unitBuildTown(destIndex.toPoint()));
	//		}
	//	}
	//	else if (me._reservedState[0]->_stateName == "한걸음")
	//	{
	//		vector2D destIndex = me._index + me.getDirectionVector(me._unitDirection);
	//		if (me.isMoveable(destIndex.toPoint()))
	//		{
	//			check = true;
	//		}
	//	}
	//	else if (me._reservedState[0]->_stateName == "NONE")
	//	{
	//		check = true;
	//	}

	//	if (check)
	//	{
	//		me.removeReserveState();
	//	}
	//	unitState* reservedState = me._reservedState[0];
	//	me._reservedState.erase(me._reservedState.begin());
	//	me.changeState(reservedState);		
	//	return;
	//}

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

		if (me._mergeUnit != nullptr)
		{
			return me.changeState(new unitMerge(me._mergeUnit,me));

		}
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

