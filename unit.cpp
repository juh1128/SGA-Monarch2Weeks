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
	gameObject::init("unit","greenKing");

	_pos.x = _pos.y = _index.x = _index.y = 0;

	this->addCallback("move", [&](tagMessage msg)
	{
		this->moveCallBack(msg.targetList[0]);
	});
	
	_unitState = new unitNoneState;
	_moveSpeed = 3.0f;
	return S_OK;
}

void unit::release()
{
}

void unit::update()
{
	gameObject::update();

	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		gameObject* temp = WORLD->getMap()->getPickedTile();
		vector<gameObject*> vr;
		vr.push_back(temp);
		this->sendMessage("move", 0, 0, 0, POINT(),vr);
		vr.clear();
	}
	_unitState->update(*this);
}

void unit::render()
{
	_scale = vector2D(CAMERA->getZoom(),CAMERA->getZoom());
	gameObject::frameRender(0,0);

}

void unit::moveCallBack(gameObject* dest)
{
	changeState(new unitOneStep((terrainTile*)dest));

}

void unit::changeState(unitState* newstate)
{
	SAFE_DELETE(_unitState);
	_unitState = newstate;
	_unitState->enter(*this);
}

void unitNoneState::update(unit & unit)
{
}

void unitOneStep::enter(unit & unit)
{
	//목적지를 못받아옴 에러 or 이상한곳찍음
	if (!_destTile) unit.changeState(new unitNoneState);

	_destPos = WORLD->getMap()->getTilePosFromIndex(_destTile->getIndex());
}

void unitOneStep::update(unit & unit)
{
	vector2D dis = _destPos - unit._pos;
	_moveRatio = WORLD->getTileMoveRatio(unit._index.x, unit._index.y);
	if (dis.getLength() <= unit._moveSpeed * _moveRatio)
	{
		unit._pos = _destPos;
		unit.changeState(new unitNoneState);
	}
	else
	{
		unit._pos = unit._pos + dis.normalize()*unit._moveSpeed*_moveRatio;

	}
}
