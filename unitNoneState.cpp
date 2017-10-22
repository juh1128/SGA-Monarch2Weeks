#include "stdafx.h"
#include "unit.h"


void unitNoneState::enter(unit & me)
{
	//me._state = UnitState::None;
	//if (me._isAuto)
	//{
		//���⿡ ���� Ÿ�� ��ĭ����
		//terrainTile* tile[2];
		//vector2D direction = me.getDirectionVector(me._unitDirection);
		//vector2D temp = me._index + direction;
		//vector2D temp2 = me._index + direction * 2;

		//tile[0] = WORLD->getMap()->getTile(temp.x, temp.y);
		//tile[1] = WORLD->getMap()->getTile(temp2.x, temp2.y);

		//���� ����->�Ǽ�->����

		//�������� ��Ȳ
		//�ڽ����κ��� 25ĭ(�����¿�밢���� 2ĭ��)
		//�ֺ��� �θ����̻��� ������� ���� ü���� �����ַκ��� ������

	//}
}


void unitNoneState::update(unit & me)
{
	////���� ���� Ȯ��
	//if (me._reservedState.size() > 0)
	//{
	//	unitState* reservedState = me._reservedState[0];
	//	//���� üũ
	//	bool check = false;
	//	if (me._reservedState[0]->_stateName == "����")
	//	{
	//		//����
	//		unit* enemy = me.isCanAttack();
	//		if (enemy != NULL)
	//		{
	//			check = true;
	//			//return me.changeState(new unitFight(enemy));
	//		}
	//	}
	//	else if (me._reservedState[0]->_stateName == "����")
	//	{
	//		if (me._mergeUnit != nullptr)
	//		{
	//			check = true;
	//			//return me.changeState(new unitMerge(me._mergeUnit, me));
	//		}
	//	}
	//	else if (me._reservedState[0]->_stateName == "�ı�")
	//	{
	//		mncObjectBase* nature = me.isCanAttackNature();
	//		if (nature != nullptr)
	//		{
	//			check = true;
	//			//return me.changeState(new unitDigObject(nature));
	//		}

	//	}
	//	//�Ǽ�
	//	else if (me._reservedState[0]->_stateName == "�Ǽ�")
	//	{
	//		vector2D destIndex = me._index + me.getDirectionVector(me._unitDirection);
	//		if (me.isBuildableTown(destIndex.toPoint()))
	//		{
	//			check = true;
	//			//return me.changeState(new unitBuildTown(destIndex.toPoint()));
	//		}
	//	}
	//	else if (me._reservedState[0]->_stateName == "�Ѱ���")
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
		//����
		//unit* dangerousUnit = me.isCanRun();
		//if (dangerousUnit != NULL)
		//{
		//	return me.changeState(new unitRun(dangerousUnit));

		//}

		//����
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
		//�Ǽ�
		vector2D destIndex = me._index + me.getDirectionVector(me._unitDirection);
		if (me.isBuildableTown(destIndex.toPoint()))
		{
			return me.changeState(new unitBuildTown(destIndex.toPoint()));
		}

		//�̵� (��� �̵����ϰ� ���� �ൿ�� �Ұ� ������ ���� ����� ���ֿ��� �����Ѵ�.)
		moveOneStep(me);
	}
	else
	{
		me._state = UnitState::Stop;
	}
}


void unitNoneState::moveOneStep(unit& me)
{
	//�ڱⰡ �ٶ󺸰� �ִ� �������� �̵��Ѵ�.
	vector2D destIndex = me._index + me.getDirectionVector(me._unitDirection);

	//1. 4���� �˻� �� �̵� ������ ������ �迭�� ��´�.
	vector<vector2D> directionCheck;
	for (int i = 0; i < 4; ++i)
	{
		vector2D dIndex = me._index + me.getDirectionVector((UnitDirection::DIRECTION)i);
		if (me.isMoveable(dIndex.toPoint()))
		{
			directionCheck.push_back(dIndex);
		}
	}

	//2. �̵� ������ ���� �� destIndex�� ������ �� �������� �̵�.
	//������ ���� �������� �̵�.
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

