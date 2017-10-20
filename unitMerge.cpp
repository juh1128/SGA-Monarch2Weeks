#include "stdafx.h"
#include "unit.h"

void unit::setMergeUnit(unit * mergeUnit)
{
	_mergeUnit = mergeUnit;
}

void unitMerge::enter(unit& me)
{
	if (!_mergeUnit) return me.changeState(new unitNoneState);

	if (me._mergeUnit == NULL)
	{
		me.setMergeUnit(_mergeUnit);
	}
	if (me._index.x == _mergeUnit->_index.x && me._index.y == _mergeUnit->_index.y)
	{
		//상대유닛이 행동중이면 상대한테 합쳐야한다

		//행동없음
		if (_mergeUnit->_isMove)
		{
			if (me._hp > _mergeUnit->_hp)
			{
				me._hp += _mergeUnit->_hp;
				_mergeUnit->setDestroy();
				me._mergeUnit = NULL;
			}
			else
			{
				_mergeUnit->_hp += me._hp;
				me.setDestroy();
				me._mergeUnit = NULL;
			}
		}
		//행동있음
		else
		{
			_mergeUnit->_hp += me._hp;
			me.setDestroy();
		}
	}
	else
	{
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
		//방향정하기
		int xDistance = UTIL::getDistance(me._index.x, 0, _mergeUnit->_index.x, 0);
		int yDistance = UTIL::getDistance(0, me._index.y, 0, _mergeUnit->_index.y);
		vector2D destIndex = me._index;
		bool isCanGo = true;

		//x가 더길어서 x로 가야하는 상황
		if(xDistance >= yDistance)
		{
			//내가 왼쪽에있다면
			if (me._index.x < _mergeUnit->_index.x)
			{
				destIndex.x += 1;
			}
			else if (me._index.x > _mergeUnit->_index.x)
			{
				destIndex.x -= 1;
			}

			if (me.isMoveable(destIndex.toPoint()))
			{
				me.changeState(new unitOneStep(destIndex.x, destIndex.y));
				return;
			}
			else
			{
				vector2D index;
				for (int i = 0; i < 4; i++)
				{
					index = me._index+UnitDirection::DIRECTION(i);
					if (me.isMoveable(index.toPoint()))
					{
						break;
					}
				}
				me.changeState(new unitOneStep(index.x, index.y));
				return;
			}

		}

		if (xDistance < yDistance)
		{
			//내가 왼쪽에있다면
			if (me._index.y < _mergeUnit->_index.y)
			{
				destIndex.y += 1;
			}
			else if (me._index.y > _mergeUnit->_index.y)
			{
				destIndex.y -= 1;
			}

			if (me.isMoveable(destIndex.toPoint()))
			{
				me.changeState(new unitOneStep(destIndex.x, destIndex.y));
				return;
			}
			else
			{
				vector2D index;
				for (int i = 0; i < 4; i++)
				{
					index = me._index + UnitDirection::DIRECTION(i);
					if (me.isMoveable(index.toPoint()))
					{
						break;
					}
				}
				me.changeState(new unitOneStep(index.x, index.y));
				return;
			}

		}
	}
}

void unitMerge::update(unit& me)
{

}