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

		//����
		judgeRun(me);

		//�ǹ����� ��Ȳ
		//judgeBuild(me);
	}
}


void unitNoneState::update(unit & me)
{
	moveOneStep(me);
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

void unitNoneState::judgeRun(unit& me)
{
	//ã�Ƴ� ���ֵ��� ���� ����
	vector<unit*> searchedUnit;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			//����ó��
			if (j == 2 && i == 2) continue;
			if (j > 0 && j < 4) continue;
			if (j + me._index.x - 2 < 0 || j + me._index.x - 2 > WORLD->getMap()->getTileCount().x) continue;
			if (i + me._index.y - 2 < 0 || i + me._index.y - 2 > WORLD->getMap()->getTileCount().y - 1) continue;

			//�ش� Ÿ�Ͽ� �ִ� ���ֺ���
			vector<unit*> unitOnTile = WORLD->getMap()->getTile(j - 2 + me._index.x, i - 2 + me._index.y)->getUnitOnTile();


			//Ÿ�Ͽ� ������ �ִٸ�
			if (unitOnTile.size() != NULL)
			{
				//Ÿ�Ͽ��ִ� ���ֺ��͸� ���� ������ ü���� ���ٸ� ����
				for (int k = 0; k <unitOnTile.size(); k++)
				{
					//�� �ڽŰ� ������ ���ٸ� ������� �ʴ´�.
					if (unitOnTile[k]->getCountryColor() == me._unitColor) continue;

					//����ü�� + 200 �� Ÿ������ ������ ü�� ���� �������
					if (me._hp + 200 < unitOnTile[k]->getHealth())
					{
						searchedUnit.push_back(unitOnTile[k]);
					}
				}
			}
		}
	}

	//����Ž���Ͽ� ���Ϳ� �� ������Ƿ� ������� �ʴٸ� ������ ã�����̴�.
	//�׷��� �������� �Լ��� �����Ѵ�.
	if (searchedUnit.size() != NULL)
	{
		//ü���� ���� ���� ������ ã�Ƴ���
		int maximumHealth = 0;
		int maximumUnitIndex = 0;
		for (int k = 0; k < searchedUnit.size(); k++)
		{
			//ã�������� �̹������� �ִ�ü�º��� ���ٸ�
			if (searchedUnit[k]->getHealth() > maximumHealth)
			{
				//�ִ�ü�°� �ִ�ü���� ���� ������ �ε����� ����
				maximumHealth = searchedUnit[k]->getHealth();
				maximumUnitIndex = k;
			}
		}

		me.changeState(new unitRun(searchedUnit[maximumUnitIndex]));

		return;
	}
}

