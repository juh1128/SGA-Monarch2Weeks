#include "stdafx.h"
#include "unit.h"
#include "mncObjectBase.h"


void unitNoneState::enter(unit & me)
{
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
	else if (me._commandDestTile)
	{
		//싸울 수 있으면 싸운다.
		unit* enemy = me.isCanAttack();
		if (enemy != NULL)
		{
			return me.changeState(new unitFight(enemy));
		}

		if (me._commandStateName == "대기")
		{
			if (vector2D(me._commandDestTile->getIndex()) == me._index)
			{
				return;
			}
		}
		else if (me._commandStateName == "자동")
		{
			if (vector2D(me._commandDestTile->getIndex()) == me._index)
			{
				me.resetCommand();
				return;
			}
		}
		else if (me._commandStateName == "파괴")
		{
			vector2D distance = vector2D(me._commandDestTile->getIndex()) - me._index;
			//해당 타일의 1칸 앞에 왔을 때
			if (distance.getLength() <= 1)
			{
				mncObjectBase* nature = me.isCanAttackNature();
				if (nature != nullptr)
				{
					me.changeState(new unitDigObject(nature));
				}
				me.resetCommand();
				return;
			}
		}
		else if (me._commandStateName == "마을 건축")
		{
			//목적지 타일이 건설 가능한 지를 확인
			if (!me.isBuildableTown(me._commandDestTile->getIndex())) return me.resetCommand();

			//목적지 타일과의 거리 계산
			vector2D distance = vector2D(me._commandDestTile->getIndex()) - me._index;
			//해당 타일의 1칸 앞에 왔을 때
			if (distance.getLength() <= 1)
			{
				me.changeState(new unitBuildTown(me._commandDestTile->getIndex()));
				me.resetCommand();
				return;
			}
		}
		else if (me._commandStateName == "목책 건축")
		{

			//목적지 타일이 건설 가능한 지를 확인
			if (!me.isBuildableTown(me._commandDestTile->getIndex())) return me.resetCommand();

			//목적지 타일과의 거리 계산
			vector2D distance = vector2D(me._commandDestTile->getIndex()) - me._index;
			//해당 타일의 1칸 앞에 왔을 때
			if (distance.getLength() <= 1)
			{
				me.changeState(new unitBuildObject(me._commandDestTile,"Wall"));
				me.resetCommand();
				return;
			}
		}
		else if (me._commandStateName == "다리 건설")
		{
			//목적지 타일에 이미 다리가 건설되었는지 확인
			mncObjectBase* onObject = (mncObjectBase*)me._commandDestTile->getObjectOnTile();
			if (onObject)
			{
				if (onObject->_name == "다리")
				{
					return me.resetCommand();
				}
			}

			//목적지 타일과의 거리 계산
			vector2D distance = vector2D(me._commandDestTile->getIndex()) - me._index;
			//해당 타일의 1칸 앞에 왔을 때
			if (distance.getLength() <= 1)
			{
				me.changeState(new unitBuildObject(me._commandDestTile, "Bridge"));
				me.resetCommand();
				return;
			}
		}

		deque<terrainTile*> path = PATHFINDER->getPath(WORLD->getMap()->getTile(me._index.x, me._index.y),
			WORLD->getMap()->getTile(me._commandDestTile->getIndex().x, me._commandDestTile->getIndex().y));
		if (path.size() > 0)
		{
			vector2D pathIndex = path[0]->getIndex();
			return me.changeState(new unitOneStep(pathIndex.x, pathIndex.y));
		}
	}


	//지시가 없으면 자동모드
	if (me._isAuto)
	{
		//도망
		unit* dangerousUnit = me.isCanRun();
		if (dangerousUnit != NULL)
		{
			return me.changeState(new unitRun(dangerousUnit));
		}

		//공격
		unit* enemy = me.isCanAttack();
		if (enemy != NULL)
		{
			return me.changeState(new unitFight(enemy));
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

