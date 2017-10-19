#include "stdafx.h"
#include "unit.h"


void unitNoneState::enter(unit & me)
{
	me._state = UnitState::None;
	if (me._isAuto)
	{
		//���⿡ ���� Ÿ�� ��ĭ����
		terrainTile* tile[2];
		vector2D direction = me.getDirectionVector(me._unitDirection);
		vector2D temp = me._index + direction;
		vector2D temp2 = me._index + direction * 2;

		tile[0] = WORLD->getMap()->getTile(temp.x, temp.y);
		tile[1] = WORLD->getMap()->getTile(temp2.x, temp2.y);

		//���� ����->�Ǽ�->����

		//�������� ��Ȳ
		//�ڽ����κ��� 25ĭ(�����¿�밢���� 2ĭ��)
		//�ֺ��� �θ����̻��� ������� ���� ü���� �����ַκ��� ������

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

