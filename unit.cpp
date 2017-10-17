#include "stdafx.h"
#include "unit.h"


unit::unit()
{
}


unit::~unit()
{
}

HRESULT unit::init(vector2D index, int height,CountryColor::Enum country)
{
	//유닛의 소속색깔
	_unitColor = country;

	string color;
	//색깔검사
	switch (country)
	{
	case CountryColor::RED:
		color = "red";
		break;
	case CountryColor::WHITE:
		color = "white";
		break;
	case CountryColor::BLUE:
		color = "blue";
		break;
	case CountryColor::GREEN:
		color = "green";
		break;
	case CountryColor::END:
		break;
	default:
		break;
	}

	gameObject::init("unit", color + "Lv1", tileMap::getTilePosFromIndex(index, height), Pivot::CENTER);

	_pos = _pos / CAMERA->getZoom();
	_index = index;
	_height = height;
	_hp = 100;
	_isStarUnit = false;

	WORLD->getMap()->getTile(_index.x, _index.y)->addUnitOnTile(this);

	_imageFrameX = 0;
	_unitDirection = (UnitDirection::DIRECTION)0;

	this->addCallback("move", [&](tagMessage msg)
	{
		this->moveCallBack(msg.ptData, msg.targetList[0]);
	});

	_isAuto = true;
	_unitState = new unitNoneState;
	_moveSpeed = 3.0f;
	_livedTime = 0;
	
	changeState(new unitNoneState);

	return S_OK;
}

void unit::release()
{
}

void unit::update()
{
	gameObject::update();

	_livedTime += TIMEMANAGER->getElapsedTime();

	imageFrame();

	auto keyboardTest = [&](UnitDirection::DIRECTION dir)
	{
		vector2D right = getDirectionVector(dir);
		vector2D dest = _index + right;

		gameObject* temp = WORLD->getMap()->getTile(dest.x, dest.y);

		vector<gameObject*> vr;
		vr.push_back(temp);

		this->sendMessage("move", 0, 0, 0, dest.toPoint(), vr);
		vr.clear();
	};

	if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
	{
		keyboardTest(UnitDirection::UNIT_RIGHT);
	}
	if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
	{
		keyboardTest(UnitDirection::UNIT_LEFT);
	}
	if (KEYMANAGER->isOnceKeyDown(VK_UP))
	{
		keyboardTest(UnitDirection::UNIT_UP);
	}
	if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
	{
		keyboardTest(UnitDirection::UNIT_DOWN);
	}
	_unitState->update(*this);


	//pos <-> 인덱스 동기화
	syncIndexFromPos();

	//Z오더 상 적절한 타일에게 렌더링을 요청한다.
	requestRender();
}

void unit::render()
{
	_scale = vector2D(CAMERA->getZoom(), CAMERA->getZoom());

	if (_image)
	{
		_image->setAlphaOption(_alpha);
		_image->setScaleOption(_scale);
		_image->frameRender(_pos.x*CAMERA->getZoom(), _pos.y*CAMERA->getZoom(), _imageFrameX, _unitDirection, _pivot);
	}
}

void unit::moveCallBack(POINT directionTile, gameObject* dest)
{
	//if (directionTile.x < 0 ||
	//	directionTile.y < 0 ||
	//	directionTile.x >= WORLD->getMap()->getTileCount().x ||
	//	directionTile.y >= WORLD->getMap()->getTileCount().y	)
	//{
	//	return changeState(new unitNoneState);
	//}
	changeState(new unitOneStep((terrainTile*)dest, directionTile));

}

void unit::changeState(unitState* newstate)
{
	SAFE_DELETE(_unitState);
	_unitState = newstate;
	_unitState->enter(*this);
}

vector2D unit::getDirectionVector(UnitDirection::DIRECTION dir)
{
	vector2D direction;

	switch (dir)
	{
	case UnitDirection::UNIT_LEFT: direction = vector2D(-1, 0);
		break;
	case UnitDirection::UNIT_UP: direction = vector2D(0, -1);
		break;
	case UnitDirection::UNIT_RIGHT:	direction = vector2D(1, 0);
		break;
	case UnitDirection::UNIT_DOWN: direction = vector2D(0, 1);
		break;
	}
	return direction;
}

void unit::syncIndexFromPos()
{
	float zoom = CAMERA->getZoom();
	_index = tileMap::getTileIndexFromPos(vector2D((_pos.x+0.1f)*zoom, (_pos.y+0.1f)*zoom), _height);
}

void unit::requestRender()
{
	//가만히 서있을 경우 다른 연산 필요없음
	if (_state != UnitState::MoveOneStep)
	{
		WORLD->getMap()->getTile(_index.x, _index.y)->requestRender(this);
		return;
	}
	
	//이동 중일 경우 캐릭터 이미지 LEFT, TOP, RIGHT, BOTTOM 부분 검사 후
	//가장 적당한 타일에 렌더링을 요청한다.
	float zoom = CAMERA->getZoom();
	vector2D zoomedPos = _pos * zoom;
	RECT rc = RectMakeCenter(zoomedPos.x, zoomedPos.y, _size.x*zoom, _size.y*zoom);

	vector2D top = vector2D(zoomedPos.x, (float)rc.top);
	vector2D right = vector2D((float)rc.right, zoomedPos.y);
	vector2D bottom = vector2D(zoomedPos.x, (float)rc.bottom);
	vector2D left = vector2D((float)rc.left, zoomedPos.y);

	vector2D indexList[4];
	indexList[0] = tileMap::getTileIndexFromPos(top, _height);
	indexList[1] = tileMap::getTileIndexFromPos(right, _height);
	indexList[2] = tileMap::getTileIndexFromPos(bottom, _height);
	indexList[3] = tileMap::getTileIndexFromPos(left, _height);

	vector2D maxIndex = indexList[0];
	for (int i = 1; i < 4; ++i)
	{
		if (maxIndex.x <= indexList[i].x && maxIndex.y <= indexList[i].y)
		{
			maxIndex = indexList[i];
		}
	}

	WORLD->getMap()->getTile(maxIndex.x, maxIndex.y)->requestRender(this);
}

void unit::build()
{
	//건설을 담당하는 함수





}

void unit::attack()
{
	//공격을 담당하는 함수
}

void unit::run(unit* starUnit)
{
	//도망가는 함수
	cout << "도망" << endl;
	terrainTile* destTile;

	int xDistance = UTIL::getDistance(_pos.x, 0, starUnit->_pos.x, 0);
	int yDistance = UTIL::getDistance(0, _pos.y, 0, starUnit->_pos.y);

	//서로의 X거리 Y거리를 구한다

	//x거리가 더 길다면
	if (abs(xDistance) > abs(yDistance))
	{
		//내가 오른쪽에있다면
		if (_index.x > starUnit->_index.x)
		{

		}
		//내가 왼쪽에있다면
		else if (_index.x < starUnit->_index.x)
		{

		}

		//갈수없다면
		if (!destTile->isWalkable() || destTile->getIndex().x < 0 || destTile->getIndex().y
			|| destTile->getIndex().x > WORLD->getMap()->getTileCount().x
			|| destTile->getIndex().y > WORLD->getMap()->getTileCount().y)
		{
			//x로 갈수없으니 y로 이동하기위함
			xDistance = 0;
			yDistance = 1;
			
		}
	}

	if (abs(xDistance) < abs(yDistance))
	{
		//내가 아래에있다면
		if (_index.y > starUnit->_index.y)
		{

		}
		//내가 위에있다면
		else if (_index.y < starUnit->_index.y)
		{

		}

		//갈수없다면
		if (!destTile->isWalkable() || destTile->getIndex().x < 0 || destTile->getIndex().y
			|| destTile->getIndex().x > WORLD->getMap()->getTileCount().x
			|| destTile->getIndex().y > WORLD->getMap()->getTileCount().y)
		{
			//x로 갈수없으니 y로 이동하기위함
			xDistance = 1;
			yDistance = 0;
		}
	}

	//한번더 x를 검사(y검사에서 바뀌었을수도있기때문)
	if (abs(xDistance) > abs(yDistance))
	{
		//내가 오른쪽에있다면
		if (_index.x > starUnit->_index.x)
		{

		}
		//내가 왼쪽에있다면
		else if (_index.x < starUnit->_index.x)
		{

		}

		//갈수없다면
		if (!destTile->isWalkable() || destTile->getIndex().x < 0 || destTile->getIndex().y
			|| destTile->getIndex().x > WORLD->getMap()->getTileCount().x
			|| destTile->getIndex().y > WORLD->getMap()->getTileCount().y)
		{
			//그냥 도망치는 대상쪽으로 가야함
		}
	}

	POINT directionIndex;
	directionIndex.x = destTile->getIndex().x;
	directionIndex.y = destTile->getIndex().y;


	changeState(new unitOneStep(destTile, directionIndex));

	//this->sendMessage("move", 0, 0, 0, _index, tileVector);
}

void unitNoneState::enter(unit & me)
{
	me._state = UnitState::None;
	if (me._isAuto)
	{
		//방향에 따른 타일 두칸검출
		terrainTile* tile[2];
		vector2D direction = me.getDirectionVector(me._unitDirection);
		vector2D temp = me._index + direction;
		vector2D temp2 = me._index + direction*2;

		tile[0] = WORLD->getMap()->getTile(temp.x, temp.y);
		tile[1] = WORLD->getMap()->getTile(temp2.x,temp2.y);

		//순서 도망->건설->공격

		//도망가는 상황
		//자신으로부터 25칸(상하좌우대각으로 2칸씩)
		//별달린 에너미가 있다면 도망가는 함수실행
		//주변에 두마리이상이 있을경우 가장 체력이 높은애로부터 도망감

		//찾아낸 유닛들
		vector<unit*> searchedUnit;
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				//예외처리
				if (j - 2 + me._index.x < 0 || j - 2 + me._index.x > WORLD->getMap()->getTileCount().x
					|| i - 2 + me._index.y < 0 || i - 2 + me._index.y > WORLD->getMap()->getTileCount().y
					|| (j== 2 && i == 2)) continue;

				//해당 타일에 있는 유닛벡터
				vector<unit*> unitOnTile = WORLD->getMap()->getTile(j - 2 + me._index.x, i - 2 + me._index.y)->getUnitOnTile();
				

				//타일에 유닛이 있다면
				if (WORLD->getMap()->getTile(j - 2 + me._index.x, i - 2 + me._index.y)->getUnitOnTile().size() != NULL)
				{
					//타일에있는 유닛벡터를 돌아 나보다 체력이 높다면 도망
					for (int k = 0; k <unitOnTile.size();k++)
					{
						//나 자신과 색깔이 같다면 계산하지 않는다.
						if (unitOnTile[k]->getCountryColor() == me._unitColor) continue;
						//그 유닛이 별달린 유닛이 아니라면
						if (!unitOnTile[k]->getIsStarUnit()) continue;

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

			me.run(searchedUnit[maximumUnitIndex]);

			return;
		}

		//건물짓는 상황
		//tile[0]번의 상하좌우대각 한칸씩을 탐색하여 건물이 없다면 건설함수 실행
		//for (int j = 0; j < 3; j++)
		//{
		//	for (int k = 0; k < 3; k++)
		//	{
		//		
		//
		//	}
		//}


		//tile를 검출하여 적이나 파괴가능한 오브젝트가 있다면 공격함수 실행
		//for (int i = 0; i < 2; i++)
		//{
		//
		//}

		//아무것도 못하면 이동한다
		vector2D dest = me._index + direction;
		POINT destp;
		destp.x = dest.x;
		destp.y = dest.y;

		if (dest.x < 0 || dest.y < 0 || dest.x >= WORLD->getMap()->getTileCount().x || dest.y >= WORLD->getMap()->getTileCount().y ||
			abs(tile[0]->getHeight() - WORLD->getMap()->getTile(me._index.x, me._index.y)->getHeight()) >= 2
			|| !WORLD->getMap()->getTile(destp.x,destp.y)->isWalkable())
			
		{
			me._unitDirection = (UnitDirection::DIRECTION)RND->getFromIntTo(0,3);
			me.changeState(new unitNoneState());
			return;
		}
		gameObject* desttile = WORLD->getMap()->getTile(dest.x, dest.y);
		vector<gameObject*> vr;
		vr.push_back(desttile);
		me.sendMessage("move", 0, 0, 0, destp, vr);
		vr.clear();
	}
}


void unitNoneState::update(unit & unit)
{
}

void unitOneStep::enter(unit & unit)
{
	unit._state = UnitState::MoveOneStep;
	//목적지를 못받아옴 에러 or 이상한곳찍음
	if (!_destTile)
	{
		return unit.changeState(new unitNoneState);
	}
	//높이 차이가 2이상 나면 이동 할 수 없음.
	if (abs(_destTile->getHeight(true) - unit._height) >= 2) return unit.changeState(new unitNoneState);

	_zoom = CAMERA->getZoom();
	_destPos = WORLD->getMap()->getTilePosFromIndex(_destTile->getIndex(), _destTile->getHeight(true)) / _zoom;
	_oldIndex = unit._index;

	//유닛 방향 설정
	unitdirection(unit);
}

void unitOneStep::update(unit & unit)
{
	if (_zoom != CAMERA->getZoom())
	{
		_destPos = WORLD->getMap()->getTilePosFromIndex(_destTile->getIndex(), _destTile->getHeight(true)) / CAMERA->getZoom();
		_zoom = CAMERA->getZoom();
	}
	vector2D dis = _destPos - unit._pos;
	_moveRatio = WORLD->getTileMoveRatio(unit._index.x, unit._index.y);

	//목적지 도착
	if (dis.getLength() <= unit._moveSpeed * _moveRatio)
	{
		WORLD->getMap()->getTile(_oldIndex.x, _oldIndex.y)->deleteUnitOnTile(&unit);
		WORLD->getMap()->getTile(_directionIndex.x, _directionIndex.y)->addUnitOnTile(&unit);
	
		unit._pos = _destPos;
		unit.changeState(new unitNoneState);

	}
	//가는 중
	else
	{
		unit._pos = unit._pos + dis.normalize()*unit._moveSpeed*_moveRatio;
		//4로 나눈 이유는 타일 중점에서 옆타일 중점 이동의 절반이기 때문 => 타일사이즈백터 길이의 절반이 이동할 거리이다
		if (dis.getLength() <= tileMap::getTileSize().getLength()*CAMERA->getZoom() *0.25f)
		{
			unit._height = _destTile->getHeight(true);
		}
	}

}
