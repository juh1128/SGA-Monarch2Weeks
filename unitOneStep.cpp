#include "stdafx.h"
#include "unit.h"


void unitOneStep::enter(unit & unit)
{
	// - 이동거리 체크 (인덱스 차이가 2칸 이상 일 경우 이동하지 않음)
	vector2D distance = _destIndex - unit._index;
	if (distance.getLength() > 1)
	{
		return unit.changeState(new unitNoneState);
	}

	_oldIndex = unit._index;
	_destTile = WORLD->getMap()->getTile(_destIndex.x, _destIndex.y);

	//유닛 방향 설정
	if (distance.x == 1)
		unit._unitDirection = UnitDirection::UNIT_RIGHT;
	else if (distance.x == -1)
		unit._unitDirection = UnitDirection::UNIT_LEFT;
	else if (distance.y == 1)
		unit._unitDirection = UnitDirection::UNIT_DOWN;
	else if (distance.y == -1)
		unit._unitDirection = UnitDirection::UNIT_UP;
	else
		cout << "유닛 방향 설정 오류" << endl;

	WORLD->getMap()->getTile(_oldIndex.x, _oldIndex.y)->deleteUnitOnTile(&unit);
	WORLD->getMap()->getTile(_destIndex.x, _destIndex.y)->addUnitOnTile(&unit);
	unit._isMove = true;
}

void unitOneStep::update(unit & me)
{
	float zoom = CAMERA->getZoom();
	vector2D destPos = tileMap::getTilePosFromIndex(_destIndex, _destTile->getHeight(true)) / zoom;
	vector2D distance = destPos - me._pos;

	//목적지 도착
	float length = distance.getLength();
	if (length <= me._moveSpeed)
	{
		//유닛이 서있는 타일 변경
		me._isMove = false;
		me._pos = destPos;

		//도착했는데 아군 유닛이 있을 경우 병합한다.
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
	//가는 중
	else
	{
		me._pos = me._pos + distance.normalize()*me._moveSpeed;

		//4로 나눈 이유는 타일 중점에서 옆타일 중점 이동의 절반이기 때문 => 타일사이즈백터 길이의 절반이 이동할 거리이다
		if (length <= tileMap::getTileSize().getLength()*zoom*0.25f)
		{
			me._height = _destTile->getHeight(true);
		}
	}

}

