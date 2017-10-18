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

	_isAuto = true;
	_unitState = new unitNoneState;
	_moveSpeed = 1.5f;
	_frameTimer = 0;
	
	changeState(new unitCreateMotion);

	return S_OK;
}

void unit::release()
{
}

void unit::update()
{
	gameObject::update();

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

bool unit::isMoveable(POINT index)
{
	//목적지가 갈 수 없는 곳일 경우 NoneState로 변경.
	vector2D tileCount = WORLD->getMap()->getTileCount();

	// - 오버플로우 체크
	if (index.x < 0 || index.x >= tileCount.x || index.y < 0 || index.y >= tileCount.y)
	{
		return false;
	}

	// - 높이, 이동불가능 타일 체크
	terrainTile* destTile = WORLD->getMap()->getTile(index.x, index.y);
	int distanceHeight = abs(destTile->getHeight(true) - _height);
	if (!destTile->isWalkable() || distanceHeight > 1)
	{
		return false;
	}

	return true;
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
	cout << "run" << endl;
	terrainTile* destTile;

	//x와y거리
	int xDistance = UTIL::getDistance(_pos.x, 0, starUnit->_pos.x, 0);
	int yDistance = UTIL::getDistance(0, _pos.y, 0, starUnit->_pos.y);

	//서로의 X거리 Y거리를 구한다
	if (abs(xDistance) == abs(yDistance))
	{
		//내가 오른쪽에있다면
		if (_pos.x > starUnit->_pos.x)
		{
			destTile = WORLD->getMap()->getTile(_index.x + 1, _index.y);
		}
		//내가 왼쪽에있다면
		else if (_pos.x < starUnit->_pos.x)
		{
			destTile = WORLD->getMap()->getTile(_index.x - 1, _index.y);
		}

		//갈수없다면
		if (!destTile->isWalkable() || destTile->getIndex().x < 0 || destTile->getIndex().y
			|| destTile->getIndex().x > WORLD->getMap()->getTileCount().x
			|| destTile->getIndex().y > WORLD->getMap()->getTileCount().y)
		{
			//내가 아래에있다면
			if (_pos.y > starUnit->_pos.y)
			{
				destTile = WORLD->getMap()->getTile(_index.x, _index.y + 1);
			}
			//내가 위에있다면
			else if (_pos.y < starUnit->_pos.y - 1)
			{
				destTile = WORLD->getMap()->getTile(_index.x, _index.y - 1);
			}
			// 갈수없다면
				if (!destTile->isWalkable() || destTile->getIndex().x < 0 || destTile->getIndex().y
					|| destTile->getIndex().x > WORLD->getMap()->getTileCount().x
					|| destTile->getIndex().y > WORLD->getMap()->getTileCount().y
					|| destTile == NULL)
				{
					destTile = WORLD->getMap()->getTile(_index.x + getDirectionVector(_unitDirection).x, _index.y + getDirectionVector(_unitDirection).y);
				}
		}
	}

	//x거리가 더 짧다면
	if (abs(xDistance) < abs(yDistance))
	{
		//내가 오른쪽에있다면
		if (_pos.x > starUnit->_pos.x)
		{
			destTile = WORLD->getMap()->getTile(_index.x + 1, _index.y);
		}
		//내가 왼쪽에있다면
		else if (_pos.x < starUnit->_pos.x)
		{
			destTile = WORLD->getMap()->getTile(_index.x - 1, _index.y);
		}
		else if (_pos.x == starUnit->_pos.x)
		{
			xDistance = 0;
			yDistance = 1;
		}

		//갈수없다면
		if (!destTile->isWalkable() || destTile->getIndex().x < 0 || destTile->getIndex().y
			|| destTile->getIndex().x > WORLD->getMap()->getTileCount().x
			|| destTile->getIndex().y > WORLD->getMap()->getTileCount().y
			|| destTile == NULL)
		{
			//x로 갈수없으니 y로 이동하기위함
			xDistance = 0;
			yDistance = 1;
		}
	}

	//y의 거리가 더 짧다면
	if (abs(xDistance) > abs(yDistance))
	{
		//내가 아래에있다면
		if (_pos.y > starUnit->_pos.y)
		{
			destTile = WORLD->getMap()->getTile(_index.x, _index.y + 1);
		}
		//내가 위에있다면
		else if (_pos.y < starUnit->_pos.y - 1)
		{
			destTile = WORLD->getMap()->getTile(_index.x, _index.y - 1);
		}

		//갈수없다면
		if (!destTile->isWalkable() || destTile->getIndex().x < 0 || destTile->getIndex().y
			|| destTile->getIndex().x > WORLD->getMap()->getTileCount().x
			|| destTile->getIndex().y > WORLD->getMap()->getTileCount().y
			|| destTile == NULL)
		{
			//x로 갈수없으니 y로 이동하기위함
			xDistance = 1;
			yDistance = 0;
		}
	}

	//한번더 x를 검사(y검사에서 바뀌었을수도있기때문)
	if (abs(xDistance) < abs(yDistance))
	{
		//내가 오른쪽에있다면
		if (_pos.x > starUnit->_pos.x)
		{
			destTile = WORLD->getMap()->getTile(_index.x + 1, _index.y);
		}
		//내가 왼쪽에있다면
		else if (_pos.x < starUnit->_pos.x)
		{
			destTile = WORLD->getMap()->getTile(_index.x - 1, _index.y);
		}
		else if (_pos.x == starUnit->_pos.x)
		{
			//내가 아래에있다면
			if (_pos.y > starUnit->_pos.y)
			{
				destTile = WORLD->getMap()->getTile(_index.x, _index.y + 1);
			}
			//내가 위에있다면
			else if (_pos.y < starUnit->_pos.y - 1)
			{
				destTile = WORLD->getMap()->getTile(_index.x, _index.y - 1);
			}
		}

		//갈수없다면
		if (!destTile->isWalkable() || destTile->getIndex().x < 0 || destTile->getIndex().y
			|| destTile->getIndex().x > WORLD->getMap()->getTileCount().x
			|| destTile->getIndex().y > WORLD->getMap()->getTileCount().y
			|| destTile == NULL)
		{
			//여기서는 x와 y를 모두 검출한거기때문에 그냥 도망을 포기한다
			//그냥 가던길로간다
			destTile = WORLD->getMap()->getTile(_index.x + getDirectionVector(_unitDirection).x, _index.y + getDirectionVector(_unitDirection).y);
		}
	}

	POINT directionIndex;
	directionIndex.x = destTile->getIndex().x;
	directionIndex.y = destTile->getIndex().y;

	if (destTile == NULL) return;
		changeState(new unitOneStep(directionIndex.x, directionIndex.y));
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

		me.run(searchedUnit[maximumUnitIndex]);

		return;
	}
}


void unitOneStep::enter(unit & unit)
{
	// - 이동거리 체크 (인덱스 차이가 2칸 이상 일 경우 이동하지 않음)
	vector2D distance = _destIndex - unit._index;
	if (distance.getLength() > 1)
	{
		unit.changeState(new unitNoneState);
	}

	unit._state = UnitState::MoveOneStep;
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

}

void unitOneStep::update(unit & unit)
{
	float zoom = CAMERA->getZoom();
	vector2D destPos = tileMap::getTilePosFromIndex(_destIndex, _destTile->getHeight(true)) / zoom;
	vector2D distance = destPos - unit._pos;

	//목적지 도착
	float length = distance.getLength();
	if (length <= unit._moveSpeed)
	{
		//유닛이 서있는 타일 변경
		WORLD->getMap()->getTile(_oldIndex.x, _oldIndex.y)->deleteUnitOnTile(&unit);
		WORLD->getMap()->getTile(_destIndex.x, _destIndex.y)->addUnitOnTile(&unit);
		unit._pos = destPos;
		unit.changeState(new unitNoneState);
	}
	//가는 중
	else
	{
		unit._pos = unit._pos + distance.normalize()*unit._moveSpeed;

		//4로 나눈 이유는 타일 중점에서 옆타일 중점 이동의 절반이기 때문 => 타일사이즈백터 길이의 절반이 이동할 거리이다
		if (length <= tileMap::getTileSize().getLength()*zoom*0.25f)
		{
			unit._height = _destTile->getHeight(true);
		}
	}

}

