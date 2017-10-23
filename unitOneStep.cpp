#include "stdafx.h"
#include "unit.h"
#include <algorithm>

void unitOneStep::enter(unit & unit)
{
	//if (unit._state != UnitState::Merge) unit._state = UnitState::Search;
	// - �̵��Ÿ� üũ (�ε��� ���̰� 2ĭ �̻� �� ��� �̵����� ����)
	vector2D distance = _destIndex - unit._index;
	if (_destIndex == unit._index) 
		cout << "�� �ڱ� Ÿ���� ���" << endl;
	float length = distance.getLength();
	if (length <= 0 || length > 1)
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

	unit._isMove = true;
	_isFinish = false;
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

		return me.changeState(new unitNoneState);
	}
	//���� ��
	else
	{
		me._pos = me._pos + distance.normalize()*me._moveSpeed;

		//4�� ���� ������ Ÿ�� �������� ��Ÿ�� ���� �̵��� �����̱� ���� => Ÿ�ϻ�������� ������ ������ �̵��� �Ÿ��̴�
		if (length <= tileMap::getTileSize().getLength()*zoom*0.25f)
		{
			if (_isFinish == false)
			{
				me._height = _destTile->getHeight(true);
				me._index = _destTile->getIndex();

				_destTile->addUnitOnTile(&me);
				me._myTiles.push_back(_destTile);
				me._myTiles[0]->deleteUnitOnTile(&me);
				me._myTiles.erase(me._myTiles.begin());

				_isFinish = true;
			}
		}
	}

}

