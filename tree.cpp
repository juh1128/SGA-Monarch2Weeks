#include "stdafx.h"
#include "tree.h"

HRESULT tree::init(int xIndex, int yIndex)
{
	terrainTile* tile = WORLD->getMap()->getTile(xIndex, yIndex);
	_height = tile->getHeight();
	tile->setObjectOnTile(this);

	gameObject::init("³ª¹«", "tree", tileMap::getTilePosFromIndex(vector2D(xIndex, yIndex), _height));
	_pos = _pos / CAMERA->getZoom();

	_index.x = xIndex;
	_index.y = yIndex;
	_frame = 0;

	return S_OK;
}

void tree::release()
{
	terrainTile* tile = WORLD->getMap()->getTile(_index.x, _index.y);
	tile->removeObjectOnTile();

	gameObject::release();
}

void tree::update()
{
	gameObject::update();
}

void tree::render()
{
	if (_image)
	{
		float zoom = CAMERA->getZoom();
		_image->setAlphaOption(_alpha);
		_image->setScaleOption(vector2D(zoom, zoom));
		_image->frameRender(_pos.x*zoom, _pos.y*zoom, _frame, 0, _pivot);
	}
}

HRESULT weed::init()
{
	return E_NOTIMPL;
}

void weed::release()
{
}

void weed::update()
{
}

void weed::render()
{
}
