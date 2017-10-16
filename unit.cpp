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

void unitNoneState::enter(unit & unit)
{
	unit._state = UnitState::None;
	if (unit._isAuto)
	{
		//방향에 따른 타일 두칸검출
		terrainTile* tile[2];
		vector2D direction = unit.getDirectionVector(unit._unitDirection);
		vector2D temp = unit._index + direction;
		vector2D temp2 = unit._index + direction*2;
		tile[0] = WORLD->getMap()->getTile(temp.x, temp.y);
		tile[1] = WORLD->getMap()->getTile(temp2.x,temp2.y);

		//순서 도망->건설->공격

		//도망가는 상황
		//자신으로부터 25칸(상하좌우대각으로 2칸씩)
		//별달린 에너미가 있다면 도망가는 함수실행
		//for (int i = 0; i < 5; i++)
		//{
		//	for (int j = 0; j < 5; j++)
		//	{
		//
		//	}
		//}

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
		vector2D dest = unit._index + direction;
		POINT destp;
		destp.x = dest.x;
		destp.y = dest.y;

		if (dest.x < 0 || dest.y < 0 || dest.x >= WORLD->getMap()->getTileCount().x || dest.y >= WORLD->getMap()->getTileCount().y ||
			abs(tile[0]->getHeight() - WORLD->getMap()->getTile(unit._index.x, unit._index.y)->getHeight()) >= 2
			//|| !WORLD->getMap()->getTile(destp.x,destp.y)->isWalkable())
			)
		{
			unit._unitDirection = (UnitDirection::DIRECTION)RND->getFromIntTo(0,3);
			unit.changeState(new unitNoneState());
			return;
		}
		gameObject* desttile = WORLD->getMap()->getTile(dest.x, dest.y);
		vector<gameObject*> vr;
		vr.push_back(desttile);
		unit.sendMessage("move", 0, 0, 0, destp, vr);
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
	if (abs(_destTile->getHeight() - unit._height) >= 2) return unit.changeState(new unitNoneState);

	_zoom = CAMERA->getZoom();
	_destPos = WORLD->getMap()->getTilePosFromIndex(_destTile->getIndex(), _destTile->getHeight()) / _zoom;
	_oldIndex = unit._index;

	//유닛 방향 설정
	unitdirection(unit);
}

void unitOneStep::update(unit & unit)
{
	if (_zoom != CAMERA->getZoom())
	{
		_destPos = WORLD->getMap()->getTilePosFromIndex(_destTile->getIndex(), _destTile->getHeight()) / CAMERA->getZoom();
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
			unit._height = _destTile->getHeight();
		}
	}

}
