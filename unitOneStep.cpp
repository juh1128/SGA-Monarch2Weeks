#include "stdafx.h"
#include "unit.h"


void unitOneStep::enter(unit & unit)
{
	// - �̵��Ÿ� üũ (�ε��� ���̰� 2ĭ �̻� �� ��� �̵����� ����)
	vector2D distance = _destIndex - unit._index;
	if (distance.getLength() > 1)
	{
		unit.changeState(new unitNoneState);
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

void unitOneStep::update(unit & unit)
{
	float zoom = CAMERA->getZoom();
	vector2D destPos = tileMap::getTilePosFromIndex(_destIndex, _destTile->getHeight(true)) / zoom;
	vector2D distance = destPos - unit._pos;

	//������ ����
	float length = distance.getLength();
	if (length <= unit._moveSpeed)
	{
		//������ ���ִ� Ÿ�� ����
		unit._isMove = false;
		unit._pos = destPos;
		unit.changeState(new unitNoneState);
	}
	//���� ��
	else
	{
		unit._pos = unit._pos + distance.normalize()*unit._moveSpeed;

		//4�� ���� ������ Ÿ�� �������� ��Ÿ�� ���� �̵��� �����̱� ���� => Ÿ�ϻ�������� ������ ������ �̵��� �Ÿ��̴�
		if (length <= tileMap::getTileSize().getLength()*zoom*0.25f)
		{
			unit._height = _destTile->getHeight(true);
		}
	}

}

