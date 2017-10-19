#include "stdafx.h"
#include "mncObjectBase.h"

HRESULT mncObjectBase::init(string name, string key, int xIndex, int yIndex, int hp, bool walkable)
{
	_index.x = xIndex;
	_index.y = yIndex;

	terrainTile* tile = WORLD->getMap()->getTile(xIndex, yIndex);
	_height = tile->getHeight();

	//�ش� x,y,height ��ġ�� Ÿ�Ͽ��� ������Ʈ�� �����.
	_walkable = walkable;
	tile->setObjectOnTile(this);
	if(!_walkable)
		tile->setWalkable(false);	//Ÿ�� �̵��Ұ��� ����

	//�Ǽ� �Ұ� ����
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
		tile->setWalkable(true);	//Ÿ�� �̵��Ұ��� ����
	tile->setBuildable(true);		//���� �Ǽ� ����

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
