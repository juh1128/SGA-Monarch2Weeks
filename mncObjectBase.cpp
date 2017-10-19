#include "stdafx.h"
#include "mncObjectBase.h"

HRESULT mncObjectBase::init(string name, string key, int xIndex, int yIndex, int hp, bool walkable)
{
	_index.x = xIndex;
	_index.y = yIndex;

	terrainTile* tile = WORLD->getMap()->getTile(xIndex, yIndex);
	_height = tile->getHeight();

	//해당 x,y,height 위치의 타일에게 오브젝트를 등록함.
	_walkable = walkable;
	tile->setObjectOnTile(this);
	if(!_walkable)
		tile->setWalkable(false);	//타일 이동불가능 설정

	//건설 불가 설정
	tile->setBuildable(false);

	gameObject::init(name, key, tileMap::getTilePosFromIndex(vector2D(xIndex, yIndex), _height));
	_pos = _pos / CAMERA->getZoom();

	_hp = hp;
	return S_OK;
}

void mncObjectBase::release()
{
	terrainTile* tile = WORLD->getMap()->getTile(_index.x, _index.y);
	tile->removeObjectOnTile();

	if (!_walkable)
		tile->setWalkable(true);	//타일 이동불가능 복구
	tile->setBuildable(true);		//이제 건설 가능

	gameObject::release();
}

void mncObjectBase::update()
{
	gameObject::update();

	if (_hp <= 0)
	{
		setDestroy();
	}
}

void mncObjectBase::render()
{
	if (_image)
	{
		float zoom = CAMERA->getZoom();
		_image->setAlphaOption(_alpha);
		_image->setScaleOption(vector2D(zoom, zoom));
		_image->render(_pos.x*zoom, _pos.y*zoom, _pivot);
	}
}

void mncObjectBase::frameRender(int xFrame, int yFrame)
{
	if (_image)
	{
		float zoom = CAMERA->getZoom();
		_image->setAlphaOption(_alpha);
		_image->setScaleOption(vector2D(zoom, zoom));
		_image->frameRender(_pos.x*zoom, _pos.y*zoom, xFrame, yFrame, _pivot);
	}
}
