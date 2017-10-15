#include "stdafx.h"
#include "unit.h"


unit::unit()
{
}


unit::~unit()
{
}

HRESULT unit::init()
{
	gameObject::init("unit", "greenKing");

	_index.y = 0;
	_index.x = 0;
	_pos.x = _pos.y = 0;
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
	WORLD->getMap()->getTile(_index.x, _index.y)->addUnitOnTile(this);
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

	if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
	{
		vector2D right(1, 0);
		vector2D dest = _index + right;
		POINT destp;
		destp.x = dest.x;
		destp.y = dest.y;
		gameObject* temp = WORLD->getMap()->getTile(dest.x, dest.y);
		vector<gameObject*> vr;
		vr.push_back(temp);
		this->sendMessage("move", 0, 0, 0, destp, vr);
		vr.clear();
	}
	if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
	{
		vector2D left(-1, 0);
		vector2D dest = _index + left;
		POINT destp;
		destp.x = dest.x;
		destp.y = dest.y;
		gameObject* temp = WORLD->getMap()->getTile(dest.x, dest.y);
		vector<gameObject*> vr;
		vr.push_back(temp);
		this->sendMessage("move", 0, 0, 0, destp, vr);
		vr.clear();
	}
	if (KEYMANAGER->isOnceKeyDown(VK_UP))
	{
		vector2D up(0, -1);
		vector2D dest = _index + up;
		POINT destp;
		destp.x = dest.x;
		destp.y = dest.y;
		gameObject* temp = WORLD->getMap()->getTile(dest.x, dest.y);
		vector<gameObject*> vr;
		vr.push_back(temp);
		this->sendMessage("move", 0, 0, 0, destp, vr);
		vr.clear();
	}
	if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
	{
		vector2D down(0, 1);
		vector2D dest = _index + down;
		POINT destp;
		destp.x = dest.x;
		destp.y = dest.y;
		gameObject* temp = WORLD->getMap()->getTile(dest.x, dest.y);
		vector<gameObject*> vr;
		vr.push_back(temp);
		this->sendMessage("move", 0, 0, 0, destp, vr);
		vr.clear();
	}
	_unitState->update(*this);
}

void unit::render()
{

	terrainTile* tile = WORLD->getMap()->getTile(_index.x, _index.y);
	int height = tile->getHeight();
	float heightUnit = tileMap::getTileSize().y * 0.5f;

	_scale = vector2D(CAMERA->getZoom(), CAMERA->getZoom());

	if (_image)
	{
		_image->setAlphaOption(_alpha);
		_image->setScaleOption(_scale);
		_image->frameRender(_pos.x*CAMERA->getZoom(), (_pos.y - height*heightUnit)*CAMERA->getZoom(), _imageFrameX, _unitDirection, _pivot);
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

vector2D unit::getunitDirection(void)
{
	vector2D direction;

	switch (_unitDirection)
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

void unitNoneState::enter(unit & unit)
{
	if (unit._isAuto)
	{
		//방향에 따른 타일 두칸검출
		terrainTile* tile[2];
		vector2D direction = unit.getunitDirection();
		vector2D temp = unit._index + direction;
		vector2D temp2 = unit._index + direction*2;
		tile[0] = WORLD->getMap()->getTile(temp.x, temp.y);
		tile[1] = WORLD->getMap()->getTile(temp2.x,temp2.y);

		for (int i = 0; i < 2; ++i)
		{
			
		}

		//아무것도 못하면 이동한다
		vector2D dest = unit._index + direction;
		POINT destp;
		destp.x = dest.x;
		destp.y = dest.y;

		if (dest.x < 0 || dest.y < 0 || dest.x >= WORLD->getMap()->getTileCount().x || dest.y >= WORLD->getMap()->getTileCount().y ||
			abs(tile[0]->getHeight() - WORLD->getMap()->getTile(unit._index.x, unit._index.y)->getHeight()) >= 2)
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

	//목적지를 못받아옴 에러 or 이상한곳찍음
	if (!_destTile)
	{
		return unit.changeState(new unitNoneState);
	}
	if (abs(_destTile->getHeight() - WORLD->getMap()->getTile(unit._index.x, unit._index.y)->getHeight()) >= 2) return unit.changeState(new unitNoneState);

	_zoom = CAMERA->getZoom();
	_destPos = WORLD->getMap()->getTilePosFromIndex(_destTile->getIndex()) / _zoom;
	_oldIndex.x = unit._index.x;
	_oldIndex.y = unit._index.y;
	WORLD->getMap()->getTile(_directionIndex.x, _directionIndex.y)->addUnitOnTile(&unit);
}

void unitOneStep::update(unit & unit)
{
	if (_zoom != CAMERA->getZoom())
	{
		_destPos = WORLD->getMap()->getTilePosFromIndex(_destTile->getIndex()) / CAMERA->getZoom();
		_zoom = CAMERA->getZoom();
	}
	vector2D dis = _destPos - unit._pos;
	_moveRatio = WORLD->getTileMoveRatio(unit._index.x, unit._index.y);

	//여기서 프레임계산
	unitdirection(unit);

	if (dis.getLength() <= unit._moveSpeed * _moveRatio)
	{

		WORLD->getMap()->getTile(_oldIndex.x, _oldIndex.y)->deleteUnitOnTile(&unit);
		unit._pos = _destPos;
		unit.changeState(new unitNoneState);
		

	}
	else
	{
		unit._pos = unit._pos + dis.normalize()*unit._moveSpeed*_moveRatio;

		//4로 나눈 이유는 타일 중점에서 옆타일 중점 이동의 절반이기 때문 => 타일사이즈백터 길이의 절반이 이동할 거리이다
		if (dis.getLength() <= tileMap::getTileSize().getLength()*CAMERA->getZoom() / 4)
		{
			unit._index = _directionIndex;
		}
	}

}
