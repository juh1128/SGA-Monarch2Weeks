#include "stdafx.h"
#include "unit.h"


void unitNoneState::enter(unit & me)
{
	me._state = UnitState::None;
	if (me._isAuto)
	{
		//방향에 따른 타일 두칸검출
		terrainTile* tile[2];
		vector2D direction = me.getDirectionVector(me._unitDirection);
		vector2D temp = me._index + direction;
		vector2D temp2 = me._index + direction * 2;

		tile[0] = WORLD->getMap()->getTile(temp.x, temp.y);
		tile[1] = WORLD->getMap()->getTile(temp2.x, temp2.y);

		//순서 도망->건설->공격

		//도망가는 상황
		//자신으로부터 25칸(상하좌우대각으로 2칸씩)
		//주변에 두마리이상이 있을경우 가장 체력이 높은애로부터 도망감

		//도망
		judgeRun(me);

		//건물짓는 상황
		//judgeBuild(me);
	}
}


void unitNoneState::update(unit & me)
{
	moveOneStep(me);
}


void unitNoneState::moveOneStep(unit& me)
{
	//자기가 바라보고 있는 방향으로 이동한다.
	vector2D destIndex = me._index + me.getDirectionVector(me._unitDirection);

	//1. 4방향 검사 후 이동 가능한 방향을 배열에 담는다.
	vector<vector2D> directionCheck;
	for (int i = 0; i < 4; ++i)
	{
		vector2D dIndex = me._index + me.getDirectionVector((UnitDirection::DIRECTION)i);
		if (me.isMoveable(dIndex.toPoint()))
		{
			directionCheck.push_back(dIndex);
		}
	}

	//2. 이동 가능한 방향 중 destIndex가 있으면 그 방향으로 이동.
	//없으면 랜덤 방향으로 이동.
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
	//찾아낸 유닛들을 담을 벡터
	vector<unit*> searchedUnit;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			//예외처리
			if (j == 2 && i == 2) continue;
			if (j > 0 && j < 4) continue;
			if (j + me._index.x - 2 < 0 || j + me._index.x - 2 > WORLD->getMap()->getTileCount().x) continue;
			if (i + me._index.y - 2 < 0 || i + me._index.y - 2 > WORLD->getMap()->getTileCount().y - 1) continue;

			//해당 타일에 있는 유닛벡터
			vector<unit*> unitOnTile = WORLD->getMap()->getTile(j - 2 + me._index.x, i - 2 + me._index.y)->getUnitOnTile();


			//타일에 유닛이 있다면
			if (unitOnTile.size() != NULL)
			{
				//타일에있는 유닛벡터를 돌아 나보다 체력이 높다면 도망
				for (int k = 0; k <unitOnTile.size(); k++)
				{
					//나 자신과 색깔이 같다면 계산하지 않는다.
					if (unitOnTile[k]->getCountryColor() == me._unitColor) continue;

					//나의체력 + 200 이 타일위의 유닛의 체력 보다 적을경우
					if (me._hp + 200 < unitOnTile[k]->getHealth())
					{
						searchedUnit.push_back(unitOnTile[k]);
					}
				}
			}
		}
	}

	//유닛탐색하여 벡터에 다 담았으므로 비어있지 않다면 유닛을 찾은것이다.
	//그래서 도망가는 함수를 실행한다.
	if (searchedUnit.size() != NULL)
	{
		//체력이 가장 높은 유닛을 찾아낸다
		int maximumHealth = 0;
		int maximumUnitIndex = 0;
		for (int k = 0; k < searchedUnit.size(); k++)
		{
			//찾은유닛중 이번유닛이 최대체력보다 높다면
			if (searchedUnit[k]->getHealth() > maximumHealth)
			{
				//최대체력과 최대체력을 가진 유닛의 인덱스를 저장
				maximumHealth = searchedUnit[k]->getHealth();
				maximumUnitIndex = k;
			}
		}

		me.changeState(new unitRun(searchedUnit[maximumUnitIndex]));

		return;
	}
}

