#include "stdafx.h"
#include "unit.h"

void unitRun::enter(unit& me)
{
	//�������� �Լ�
	cout << "run" << endl;
	terrainTile* destTile;
	POINT destIndex;
	destIndex.x = me._index.x;
	destIndex.y = me._index.y;

	//x��y�Ÿ�
	POINT distance;
	distance.x = UTIL::getDistance(me._index.x, 0, _avoidUnit->_index.x, 0);
	distance.y = UTIL::getDistance(0, me._index.y, 0, _avoidUnit->_index.y);

	//if (me.isMoveable(destIndex))
	//{
	//
	//}
	//else
	//{
	//
	//}

	//������ X�Ÿ� Y�Ÿ��� ���ٸ�
	if (distance.x == distance.y)
	{
		bool isCanGo = true;
		//���� ���̰� ���ٸ� x������ ���� �̵�
		//�� ������ ���ʿ��ִٸ� �����̵��ؾ���
		if (me._index.x <= _avoidUnit->_index.x)
		{
			destIndex.x -= 1;
		}
		//�� ������ �����ʿ� �ִٸ� ���������� �̵��ؾ���
		else if (me._index.x > _avoidUnit->_index.x)
		{
			destIndex.x += 1;
		}

		if (WORLD->getMap()->getTile(destIndex.x, destIndex.y)->isWalkable())
		{
			if (destIndex.x < 0 || destIndex.y <0
				|| destIndex.y > WORLD->getMap()->getTileCount().y
				|| destIndex.x > WORLD->getMap()->getTileCount().x)
			{
				isCanGo = false;
			}
		}
		else
		{
			isCanGo = false;
		}

		//�����ߴ� Ÿ���� �����ٸ� y������ �̵��ؾ��Ѵ�
		if (!isCanGo)
		{
			isCanGo = true;
			destIndex.x = me._index.x;
			destIndex.y = me._index.y;
			//�� ������ �����ִٸ�
			if (me._index.y <= _avoidUnit->_index.y)
			{
				destIndex.y -= 1;
			}
			else if (me._index.y > _avoidUnit->_index.y)
			{
				destIndex.y += 1;
			}

			//�����ִ�Ÿ���ε�
			if (WORLD->getMap()->getTile(destIndex.x, destIndex.y)->isWalkable())
			{
		
			}
			//��������Ÿ���̸�
			else
			{
				//����
				isCanGo = false;
			}
		}

		//x��y�� ���Ҵµ��� ������ �� ���������� ������ �˾Ƽ� �ϰ��� ��
		if (!isCanGo)
		{
			for (int i = 0; i < 4; i++)
			{	
				//����������
				if (WORLD->getMap()->getTile(me._index.x + me.getDirectionVector((UnitDirection::DIRECTION)i).x, me._index.y + me.getDirectionVector((UnitDirection::DIRECTION)i).y))
				{

				}
			}
		}
	}

	//x�Ÿ��� �� ª�ٸ�
	if (distance.x < distance.y)
	{
		
	}

	//y�� �Ÿ��� �� ª�ٸ�
	if (distance.x > distance.y)
	{
		
	}

	//POINT directionIndex;
	//directionIndex.x = destTile->getIndex().x;
	//directionIndex.y = destTile->getIndex().y;
	//
	//me.changeState(new unitOneStep(directionIndex.x, directionIndex.y));
}

