#include "stdafx.h"
#include "unit.h"
#include "mncObjectBase.h"
#include "objectFactory.h"

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
	string color = getColorString();

	gameObject::init("unit", color + "Lv1", tileMap::getTilePosFromIndex(index, height), Pivot::CENTER);
	this->setImage(objectFactory::getUnitImage(country, 1), objectFactory::getUnitImage(country, 2),
		objectFactory::getUnitImage(country, 3));

	_pos = _pos / CAMERA->getZoom();
	_index = index;

	_hp = 100;
	_isStarUnit = false;

	terrainTile* onTile = WORLD->getMap()->getTile(_index.x, _index.y);
	_height = onTile->getHeight();
	onTile->addUnitOnTile(this);
	_myTiles.push_back(onTile);

	_imageFrameX = 0;
	_unitDirection = (UnitDirection::DIRECTION)(RND->getFromIntTo(0, 3));

	_isAuto = true;
	_unitState = new unitNoneState;
	_moveSpeed = 1.5f;
	_frameTimer = 0;
	_mergeUnit = NULL;

	_commandDestTile = NULL;
	_commandTargetUnit = NULL;
	
	changeState(new unitCreateMotion);

	//this->addCallback("대기", [&](tagMessage msg) {
	//	terrainTile* tile = (terrainTile*)msg.targetList[0];
	//	this->moveAstar(tile->getIndex().x, tile->getIndex().y);
	//	this->reserveState(new unitNoneState);
	//	this->setAuto(false);
	//});

	//this->addCallback("자동", [&](tagMessage msg) {
	//	terrainTile* tile = (terrainTile*)msg.targetList[0];
	//	this->moveAstar(tile->getIndex().x, tile->getIndex().y);
	//	this->reserveState(new unitNoneState);
	//	this->setAuto(true);
	//});

	//this->addCallback("마을 건축", [&](tagMessage msg) {
	//	terrainTile* tile = (terrainTile*)msg.targetList[0];
	//	this->moveAstar(tile->getIndex().x, tile->getIndex().y);
	//	this->reserveState(new unitBuildTown(msg.ptData));
	//	this->setAuto(true);
	//});
	//this->addCallback("목책 건축", [&](tagMessage msg) {
	//	terrainTile* tile = (terrainTile*)msg.targetList[0];
	//	this->moveAstar(tile->getIndex().x, tile->getIndex().y);
	//	this->reserveState(new unitNoneState);
	//	this->setAuto(true);
	//});
	//this->addCallback("다리 건설", [&](tagMessage msg) {
	//	terrainTile* tile = (terrainTile*)msg.targetList[0];
	//	this->moveAstar(tile->getIndex().x, tile->getIndex().y);
	//	this->reserveState(new unitNoneState);
	//	this->setAuto(true);
	//});

	//==================================================
	// ## 원군 명령
	this->addCallback("원군", [&](tagMessage msg) {	
		unit* target = (unit*)msg.targetList[0];
		if (target == this)
		{
			return;
		}
		//대상에게 Merge 메시지가 발동하면, 나한테 알려달라고 등록한다.
		//기존에 관찰자 등록을 했던 타겟이 있으면, 해제한다.
		target->addObserver("Merge", this);

		unit* oldTarget = this->getCommandTarget();
		if (oldTarget)
			oldTarget->removeObserver("Merge", this);

		this->setCommand(NULL, target, "원군");

	});
	//==================================================
	// ## 추적 명령
	this->addCallback("추적", [&](tagMessage msg) {
		unit* target = (unit*)msg.targetList[0];
		if (target == this) return;		

		target->addObserver("Merge", this);
		unit* oldTarget = this->getCommandTarget();
		if (oldTarget)
			oldTarget->removeObserver("Merge", this);

		this->setCommand(NULL, target, "추적");
	});
	//==================================================
	// ## 쫒아가던 유닛이 병합되어서 사라졌을 경우 처리
	this->addCallback("observed_Merge", [this](tagMessage msg)
	{
		//자기가 쫒아가던 유닛이 병합되서 사라졌을 경우, 다른 대상으로 변경한다.
		this->setCommand(NULL, (unit*)msg.targetList[0], this->getCommandName());
		msg.targetList[0]->addObserver("Merge", this);
	});

	return S_OK;
}

void unit::release()
{
	//유닛이 삭제될 때 자기가 서있는 타일에 등록 해제함.
	for (size_t i = 0; i < _myTiles.size(); ++i)
	{
		_myTiles[i]->deleteUnitOnTile(this);
	}

	gameObject::release();
}

void unit::update()
{

	gameObject::update();

	imageFrame();

	if (_commandTargetUnit)
	{
		if (!_commandTargetUnit->isLive())
		{
			resetCommand();
		}
	}

	_unitState->update(*this);

	//Z오더 상 적절한 타일에게 렌더링을 요청한다.
	requestRender();
	if (_hp <= 0)
	{
		return this->setDestroy();
	}
}

void unit::render()
{
	if (_image)
	{
		float zoom = CAMERA->getZoom();
		float imageHalfHeight = _image->getFrameSize(_imageFrameX).y * zoom * 0.5f;

		_scale = vector2D(zoom, zoom);

		_image->setAlphaOption(_alpha);
		_image->setScaleOption(_scale);
		_image->frameRender(_pos.x*zoom, _pos.y*zoom + imageHalfHeight-10, _imageFrameX, _unitDirection, _pivot);

		vector2D renderPos = _pos*zoom;
		renderPos = CAMERA->getRelativeVector2D(renderPos);

		if (_commandStateName != "")
		{
			IMAGEMANAGER->drawText(renderPos.x, renderPos.y, UTIL::string_to_wstring(_commandStateName), 14, DefaultBrush::white,
				DWRITE_TEXT_ALIGNMENT_LEADING);
		}
		else if (_state == UnitState::Search)
		{
			IMAGEMANAGER->drawText(renderPos.x, renderPos.y, L"탐색", 14, DefaultBrush::white,
				DWRITE_TEXT_ALIGNMENT_LEADING);
		}
		else if (_state == UnitState::Run)
		{		
			IMAGEMANAGER->drawText(renderPos.x, renderPos.y, L"도망", 14, DefaultBrush::white,
				DWRITE_TEXT_ALIGNMENT_LEADING);
		}
		else if (_state == UnitState::Fight)
		{
			IMAGEMANAGER->drawText(renderPos.x, renderPos.y, L"전투", 14, DefaultBrush::white,
				DWRITE_TEXT_ALIGNMENT_LEADING);
		}
		else if (_state == UnitState::Stop)
		{
			IMAGEMANAGER->drawText(renderPos.x, renderPos.y, L"대기", 14, DefaultBrush::white,
				DWRITE_TEXT_ALIGNMENT_LEADING);
		}
		else if (_state == UnitState::BuildTown)
		{
			IMAGEMANAGER->drawText(renderPos.x, renderPos.y, L"건설", 14, DefaultBrush::white,
				DWRITE_TEXT_ALIGNMENT_LEADING);
		}
		else if (_state == UnitState::Merge)
		{
			IMAGEMANAGER->drawText(renderPos.x, renderPos.y, L"원군", 14, DefaultBrush::white,
				DWRITE_TEXT_ALIGNMENT_LEADING);
		}

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
	_image = setUnitLvImage(_hp);

	//가만히 서있을 경우 다른 연산 필요없음
	if (!_isMove)
	{
		WORLD->getMap()->getTile(_index.x, _index.y)->requestRender(this);
		return;
	}
	
	//이동 중일 경우 캐릭터 이미지 LEFT, TOP, RIGHT, BOTTOM 부분 검사 후
	//가장 적당한 타일에 렌더링을 요청한다.
	float zoom = CAMERA->getZoom();
	vector2D frameSize = _image->getFrameSize(_imageFrameX);
	float imageHalfHeight = frameSize.y * zoom * 0.5f;
	vector2D zoomedPos = _pos * zoom;
	RECT rc = RectMakeCenter(zoomedPos.x, zoomedPos.y + imageHalfHeight - 10, frameSize.x*zoom, frameSize.y*zoom);

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

image* unit::setUnitLvImage(int health)
{
	if (health < 1000)
		return  _lv[1];
	else if (health < 10000)
		return  _lv[2];
	else if (health >= 10000)
		return _lv[3];
	else
	{
		cout << "setUnitLvImage오류 " << endl;
	}
}
void unit::setImage(image* lv1, image* lv2, image* lv3)
{
	_lv[1] = lv1;
	_lv[2] = lv2;
	_lv[3] = lv3;
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

	// - 이동 하려는 타일에 적 유닛이 있는지 체크
	vector<unit*> vr = destTile->getUnitOnTile();
	if (vr.size() >= 0)
	{
		for (int i = 0; i < vr.size(); ++i)
		{
			if (vr[i]->getCountryColor() != this->getCountryColor())
			{
				return false;
			}
		}
	}

	return true;
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
			if (j + _index.x - 2 < 0 || j + _index.x - 2 > WORLD->getMap()->getTileCount().x - 1) continue;
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
					if (_hp < unitOnTile[k]->getHealth())
					{
						if (abs(unitOnTile[k]->getHealth() - _hp) > 200)
						{
							searchedUnit.push_back(unitOnTile[k]);
						}
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



string unit::getColorString()
{
	string color;
	switch (_unitColor)
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
	return color;
}
