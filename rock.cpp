#include "stdafx.h"
#include "rock.h"

HRESULT rock::init(int xIndex, int yIndex)
{
	terrainTile* tile = WORLD->getMap()->getTile(xIndex, yIndex);
	_height = tile->getHeight();
	tile->setWalkable(false);	//타일 이동불가능 설정

	tile->setObjectOnTile(this);

	gameObject::init("돌", "rock", tileMap::getTilePosFromIndex(vector2D(xIndex, yIndex), _height));
	_pos = _pos / CAMERA->getZoom();

	_index.x = xIndex;
	_index.y = yIndex;
	return S_OK;
}

void rock::release()
{
	terrainTile* tile = WORLD->getMap()->getTile(_index.x, _index.y);
	tile->removeObjectOnTile();
	tile->setWalkable(true);

	gameObject::release();
}

void rock::update()
{
	gameObject::update();
}

void rock::render()
{
	if (_image)
	{
		float zoom = CAMERA->getZoom();
		_image->setAlphaOption(_alpha);
		_image->setScaleOption(vector2D(zoom, zoom));
		_image->render(_pos.x*zoom, _pos.y*zoom, _pivot);
	}
}
