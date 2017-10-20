#include "stdafx.h"
#include "unit.h"


void unitOneStep::enter(unit & unit)
{
	// - �̵��Ÿ� üũ (�ε��� ���̰� 2ĭ �̻� �� ��� �̵����� ����)
	vector2D distance = _destIndex - unit._index;
	if (distance.getLength() > 1)
	{
		return unit.changeState(new unitNoneState);
	}

	_oldIndex = unit._index;
	_destTile = WORLD->getMap()->getTile(_destIndex.x, _destIndex.y);

	//���� ���� ����
	if (distance.x == 1)
		unit._unitDirection = UnitDirection::UNIT_RIGHT;
	else if (distance.x == -1)
		unit._unitDirection = UnitDirection::UNIT_LEFT;
	else if (distance.y == 1)
		unit._unitDirection = UnitDirection::UNIT_DOWN;
	else if (distance.y == -1)
		unit._unitDirection = UnitDirection::UNIT_UP;
	else
		cout << "���� ���� ���� ����" << endl;

	WORLD->getMap()->getTile(_oldIndex.x, _oldIndex.y)->deleteUnitOnTile(&unit);
	WORLD->getMap()->getTile(_destIndex.x, _destIndex.y)->addUnitOnTile(&unit);
	unit._isMove = true;
}

void unitOneStep::update(unit & me)
{
	float zoom = CAMERA->getZoom();
	vector2D destPos = tileMap::getTilePosFromIndex(_destIndex, _destTile->getHeight(true)) / zoom;
	vector2D distance = destPos - me._pos;

	//������ ����
	float length = distance.getLength();
	if (length <= me._moveSpeed)
	{
		//������ ���ִ� Ÿ�� ����
		me._isMove = false;
		me._pos = destPos;

		//�����ߴµ� �Ʊ� ������ ���� ��� �����Ѵ�.
		vector<unit*> unitList = _destTile->getUnitOnTile();
		bool isMerge = false;
		for (size_t i = 0; i < unitList.size(); ++i)
		{
			if (unitList[i]->isLive())
			{
				if (unitList[i] != &me && unitList[i]->getCountryColor() == me._unitColor)
				{
					me.changeState(new unitMerge(unitList[i]));
					isMerge = true;
					return;
				}
			}
		}

		if(me._mergeUnit)
			return me.changeState(new unitMerge(me._mergeUnit));

		if(!isMerge)
			return me.changeState(new unitNoneState);
	}
	//���� ��
	else
	{
		me._pos = me._pos + distance.normalize()*me._moveSpeed;

		//4�� ���� ������ Ÿ�� �������� ��Ÿ�� ���� �̵��� �����̱� ���� => Ÿ�ϻ�������� ������ ������ �̵��� �Ÿ��̴�
		if (length <= tileMap::getTileSize().getLength()*zoom*0.25f)
		{
			me._height = _destTile->getHeight(true);
		}
	}

}

