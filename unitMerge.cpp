#include "stdafx.h"
#include "unit.h"

unit* unit::isCanMerge(unit* mergeunit)
{
	_mergeUnit = mergeunit;
	return _mergeUnit;




	cout << "nullptr 반환" << endl;
	return nullptr;
}

void unitMerge::enter(unit& me)
{
	me._state = UnitState::Merge;

	if (me._mergeUnit == NULL)
	{
		return me.changeState(new unitNoneState);
	}

	if (!me._mergeUnit->isLive())
	{
		me._mergeUnit = NULL;
		return me.changeState(new unitNoneState);
	}


	me.syncIndexFromPos();
	me._mergeUnit->syncIndexFromPos();
	
	if (me._index.x == me._mergeUnit->_index.x && me._index.y == me._mergeUnit->_index.y)
	{
		//상대유닛이 행동중이면 상대한테 합쳐야한다
		auto merge = [](unit& destroyUnit, unit& addedUnit)
		{
			addedUnit.setHp(addedUnit.getHealth() + destroyUnit.getHealth());
			destroyUnit.setDestroy();
		};

		//행동없음
		if (me._mergeUnit->_isMove)
		{
			if (me._hp > me._mergeUnit->_hp)
			{
				merge(*me._mergeUnit, me);
				me._mergeUnit = NULL;
			}
			else
			{
				merge(me, *me._mergeUnit);
				me._mergeUnit = NULL;
			}
		}
		//행동있음
		else
		{
			merge(me, *me._mergeUnit);
			me._mergeUnit = NULL;
		}
	}
	else
	{
		//======================================
		// - 병합하려했는데 도망가야되거나 싸울 상황일 경우

		unit* runTarget = me.isCanRun();
		if (runTarget)
		{
			me.changeState(new unitRun(runTarget));
			return;
		}
		unit* attackTarget = me.isCanAttack();
		if (attackTarget)
		{
			me.changeState(new unitFight(attackTarget));
			return;
		}

		////====================================
		//// - 병합할 대상에게 추적하는 상황

		//머지 상대의 인덱스가 바뀌는지 체크할것 코딩해야됨

		//me.moveAstar(me._mergeUnit->_index.x,me._mergeUnit->_index.y);

		////방향정하기
		//int xDistance = UTIL::getDistance(me._index.x, 0, me._mergeUnit->_index.x, 0);
		//int yDistance = UTIL::getDistance(0, me._index.y, 0, me._mergeUnit->_index.y);
		//vector2D destIndex = me._index;
		//bool isCanGo = true;

		////x가 더길어서 x로 가야하는 상황
		//if(xDistance >= yDistance)
		//{
		//	//내가 왼쪽에있다면
		//	if (me._index.x < me._mergeUnit->_index.x)
		//	{
		//		destIndex.x += 1;
		//	}
		//	else if (me._index.x > me._mergeUnit->_index.x)
		//	{
		//		destIndex.x -= 1;
		//	}

		//	if (me.isMoveable(destIndex.toPoint()))
		//	{
		//		me.changeState(new unitOneStep(destIndex.x, destIndex.y));
		//		return;
		//	}
		//	else
		//	{
		//		vector2D index;
		//		for (int i = 0; i < 4; i++)
		//		{
		//			index = me._index+UnitDirection::DIRECTION(i);
		//			if (me.isMoveable(index.toPoint()))
		//			{
		//				break;
		//			}
		//		}
		//		me.changeState(new unitOneStep(index.x, index.y));
		//		return;
		//	}

		//}

		//if (xDistance < yDistance)
		//{
		//	//내가 왼쪽에있다면
		//	if (me._index.y < me._mergeUnit->_index.y)
		//	{
		//		destIndex.y += 1;
		//	}
		//	else if (me._index.y > me._mergeUnit->_index.y)
		//	{
		//		destIndex.y -= 1;
		//	}

		//	if (me.isMoveable(destIndex.toPoint()))
		//	{
		//		me.changeState(new unitOneStep(destIndex.x, destIndex.y));
		//		return;
		//	}
		//	else
		//	{
		//		vector2D index;
		//		for (int i = 0; i < 4; i++)
		//		{
		//			index = me._index + UnitDirection::DIRECTION(i);
		//			if (me.isMoveable(index.toPoint()))
		//			{
		//				break;
		//			}
		//		}
		//		me.changeState(new unitOneStep(index.x, index.y));
		//		return;
		//	}

		//}

	}
}

void unitMerge::update(unit& me)
{
	me.changeState(new unitNoneState);
}