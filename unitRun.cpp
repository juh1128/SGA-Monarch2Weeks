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

		//������Ÿ���̸�
		if (!me.isMoveable(destIndex))
		{
			//����
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

			//�������� Ÿ���̸�
			if (!me.isMoveable(destIndex))
			{
				//����
				isCanGo = false;
			}
		}

		//x��y�� ���Ҵµ��� ������ �� ���⼭ �����ִ�Ÿ��ã��
		if (!isCanGo)
		{
			vector2D index;
			for (int i = 0; i < 4; i++)
			{	
				index = me._index + me.getDirectionVector(UnitDirection::DIRECTION(i));
				if (me.isMoveable(index.toPoint()))
				{
					break;
				}
			}
			me.changeState(new unitOneStep(index.x,index.y));
		}
		else me.changeState(new unitOneStep(destIndex.x, destIndex.y));
	}

	//x�Ÿ��� �� ª�ٸ�
	if (distance.x < distance.y)
	{
		bool isCanGo = true;
		//���� ���ʿ��ִٸ�
		if (me._index.x < _avoidUnit->_index.x)
		{
			destIndex.x -= 1;
		}
		else if (me._index.x > _avoidUnit->_index.x)
		{
			destIndex.x += 1;
		}

		//�����ִٸ�
		if (me.isMoveable(destIndex))
		{
			cout << "�¿��̵����� ����" << endl;
			me.changeState(new unitOneStep(destIndex.x,destIndex.y));
		}
		//�������ٸ�
		else
		{
			vector2D index;
			for (int i = 0; i < 4; i++)
			{
				index = me.getDirectionVector(UnitDirection::DIRECTION(i));
				if (me.isMoveable(index.toPoint()))
				{
					return;
				}
			}
			cout << "�¿��̵����� ����" << endl;
			me.changeState(new unitOneStep(index.x, index.y));
		}
	}

	//y�� �Ÿ��� �� ª�ٸ�
	if (distance.x > distance.y)
	{
		bool isCanGo = true;
		//���� ���ʿ��ִٸ�
		if (me._index.y < _avoidUnit->_index.y)
		{
			destIndex.y -= 1;
		}
		else if (me._index.y > _avoidUnit->_index.y)
		{
			destIndex.y += 1;
		}

		//�����ִٸ�
		if (me.isMoveable(destIndex))
		{
			cout << "�����̵����� ����" << endl;
			me.changeState(new unitOneStep(destIndex.x, destIndex.y));
		}
		//�������ٸ�
		else
		{
			vector2D index;
			for (int i = 0; i < 4; i++)
			{
				index = me.getDirectionVector(UnitDirection::DIRECTION(i));
				if (me.isMoveable(index.toPoint()))
				{
					return;
				}
			}
			cout << "�����̵����� ����" << endl;
			me.changeState(new unitOneStep(index.x, index.y));
		}
	}
}