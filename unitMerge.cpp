#include "stdafx.h"
#include "unit.h"

unit* unit::isCanMerge(unit* mergeunit)
{
	_mergeUnit = mergeunit;
	return _mergeUnit;




	cout << "nullptr ��ȯ" << endl;
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
		//��������� �ൿ���̸� ������� ���ľ��Ѵ�
		auto merge = [](unit& destroyUnit, unit& addedUnit)
		{
			addedUnit.setHp(addedUnit.getHealth() + destroyUnit.getHealth());
			destroyUnit.setDestroy();
		};

		//�ൿ����
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
		//�ൿ����
		else
		{
			merge(me, *me._mergeUnit);
			me._mergeUnit = NULL;
		}
	}
	else
	{
		//======================================
		// - �����Ϸ��ߴµ� �������ߵǰų� �ο� ��Ȳ�� ���

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
		//// - ������ ��󿡰� �����ϴ� ��Ȳ

		//���� ����� �ε����� �ٲ���� üũ�Ұ� �ڵ��ؾߵ�

		//me.moveAstar(me._mergeUnit->_index.x,me._mergeUnit->_index.y);

		////�������ϱ�
		//int xDistance = UTIL::getDistance(me._index.x, 0, me._mergeUnit->_index.x, 0);
		//int yDistance = UTIL::getDistance(0, me._index.y, 0, me._mergeUnit->_index.y);
		//vector2D destIndex = me._index;
		//bool isCanGo = true;

		////x�� ���� x�� �����ϴ� ��Ȳ
		//if(xDistance >= yDistance)
		//{
		//	//���� ���ʿ��ִٸ�
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
		//	//���� ���ʿ��ִٸ�
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