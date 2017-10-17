#include "stdafx.h"
#include "wall.h"

HRESULT wall::init(int xIndex, int yIndex)
{
	terrainTile* tile = WORLD->getMap()->getTile(xIndex, yIndex);
	_height = tile->getHeight();
	tile->setObjectOnTile(this);
	tile->setWalkable(false);	//타일 이동불가능 설정

	gameObject::init("목책", "wall", tileMap::getTilePosFromIndex(vector2D(xIndex, yIndex), _height));
	_pos = _pos / CAMERA->getZoom();

	_index.x = xIndex;
	_index.y = yIndex;
	_frame = 0;

	_hp = 200;

	return S_OK;
}

void wall::release()
{
	terrainTile* tile = WORLD->getMap()->getTile(_index.x, _index.y);
	tile->removeObjectOnTile();
	tile->setWalkable(false);	

	gameObject::release();
}

void wall::update()
{
	gameObject::update();
}

void wall::render()
{
	if (_image)
	{
		float zoom = CAMERA->getZoom();
		_image->setAlphaOption(_alpha);
		_image->setScaleOption(vector2D(zoom, zoom));
		_image->frameRender(_pos.x*zoom, _pos.y*zoom, _frame, 0, _pivot);
	}
}
