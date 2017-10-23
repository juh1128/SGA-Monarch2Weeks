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
	//�÷��̾�κ��� ���� ���ð� ���� ���
	// - ���� ���� ���
	if (me._commandTargetUnit)
	{		
		if (me._commandStateName == "����")
		{
			vector2D distance = me._commandTargetUnit->_index - me._index;
			//�ش� ������ 1ĭ �տ� ���� ��
			if (distance.getLength() <= 1)
			{
				me.changeState(new unitFight(me._commandTargetUnit));
				me.resetCommand();
				return;
			}
		}
		else if(me._commandStateName == "����")
		{
			if (me._commandTargetUnit->_index == me._index)
			{
				me.resetCommand();
				return;
			}
		}
		
		//�ο� �� ������ �ο��.
		unit* enemy = me.isCanAttack();
		if (enemy != NULL)
		{
			return me.changeState(new unitFight(enemy));
		}

		//��ã�� (���� ����)
		deque<terrainTile*> path = PATHFINDER->getPath(WORLD->getMap()->getTile(me._index.x, me._index.y), 
			WORLD->getMap()->getTile(me._commandTargetUnit->_index.x, me._commandTargetUnit->_index.y));
		if (path.size() > 0)
		{
			vector2D pathIndex = path[0]->getIndex();
			me.changeState(new unitOneStep(pathIndex.x, pathIndex.y));
		}
		else
		{
			cout << "�� ��ã��" << endl;
			me.resetCommand();
		}
		return;
	}
	// - Ÿ�� ���� ���
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


	//���ð� ������ �ڵ����
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

