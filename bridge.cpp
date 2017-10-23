#include "stdafx.h"
#include "bridge.h"
#include "unit.h"

HRESULT bridge::init(int xIndex, int yIndex)
{
	mncObjectBase::init("�ٸ�", "bridge", xIndex, yIndex, 100, true);
	_cost = 2000;

	_frame = 0;

	//�ٸ� ���� üũ direction: 0�̸� ����, 1�̸� ������
	auto isMovable = [](POINT index, int height)
	{
		vector2D tileCount = WORLD->getMap()->getTileCount();

		// - �����÷ο� üũ
		if (index.x < 0 || index.x >= tileCount.x || index.y < 0 || index.y >= tileCount.y)
		{
			return false;
		}

		// - ����, �̵��Ұ��� Ÿ�� üũ
		terrainTile* destTile = WORLD->getMap()->getTile(index.x, index.y);
		int distanceHeight = abs(destTile->getHeight(true) - height);
		if (!destTile->isWalkable() || distanceHeight > 1)
		{
			return false;
		}

		return true;
	};

	//����
	if (isMovable(PointMake(xIndex-1, yIndex), _height) && isMovable(PointMake(xIndex+1, yIndex), _height))
	{
		_direction = 0;
	}
	else if (isMovable(PointMake(xIndex, yIndex - 1), _height) && isMovable(PointMake(xIndex, yIndex + 1), _height))
	{
		_direction = 1;
	}

	return S_OK;
}

void bridge::release()
{
	mncObjectBase::release();
	terrainTile* tile = WORLD->getMap()->getTile(_index.x, _index.y);
	tile->setBuildable(false);	
}

void bridge::update()
{
	mncObjectBase::update();

	//ü�¿� ���� ������ ����
	float hpRatio = (float)_hp / (float)_maxHp;
	if (hpRatio > 0.33f)
	{
		_frame = 0;
	}
	else
	{
		_frame = 1;
	}
}

void bridge::render()
{
	int renderFrame = _direction + _frame;
	if (renderFrame > 3) renderFrame = 3;

	float zoom = CAMERA->getZoom();
	float imageHalfHeight = _image->getFrameSize(0).y * zoom * 0.5f;

	_image->setAlphaOption(_alpha);
	_image->setScaleOption(vector2D(zoom, zoom));
	_image->frameRender((_pos.x)*zoom, (_pos.y-10)*zoom + imageHalfHeight, renderFrame, 0, Pivot::CENTER);

}
