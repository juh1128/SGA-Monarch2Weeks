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

	this->addCallback("move", [this](tagMessage msg)
	{
		this->moveCallBack(msg.ptData);
	});
	

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
		POINT temp = WORLD->getMap()->getPickedTile()->getIndex();
		this->sendMessage("move", 0, 0, 0, (POINT)temp);
	}
}

void unit::render()
{
	_scale = vector2D(CAMERA->getZoom(),CAMERA->getZoom());
	gameObject::frameRender(0,0);

}

void unit::moveCallBack(POINT dest)
{
	_index =  (vector2D)dest;
	_pos = tileMap::getTilePosFromIndex(_index);
}
