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
	gameObject::release();
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

unit* unit::isCanRun()
{
	vector<unit*>searchedUnit;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			//예외처리
			if (i > 0 && i < 4 && j > 0 && j < 4) continue;
			if (j + _index.x - 2 < 0 || j + _index.x - 2 > WORLD->getMap()->getTileCount().x) continue;
			if (i + _index.y - 2 < 0 || i + _index.y - 2 > WORLD->getMap()->getTileCount().y - 1) continue;

			//해당 타일에 있는 유닛벡터
			vector<unit*> unitOnTile = WORLD->getMap()->getTile(j - 2 + _index.x, i - 2 + _index.y)->getUnitOnTile();


			//타일에 유닛이 있다면
			if (unitOnTile.size() != NULL)
			{
				//타일에있는 유닛벡터를 돌아 나보다 체력이 높다면 도망
				for (int k = 0; k <unitOnTile.size(); k++)
				{
					//나 자신과 색깔이 같다면 계산하지 않는다.
					if (unitOnTile[k]->getCountryColor() == _unitColor) continue;

					//나의체력 + 200 이 타일위의 유닛의 체력 보다 적을경우
					if (_hp + 200 < unitOnTile[k]->getHealth())
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
	
		return searchedUnit[maximumUnitIndex];
	}
	else return NULL;
}







