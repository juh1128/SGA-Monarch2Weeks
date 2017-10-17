#include "stdafx.h"
#include "town.h"
#include "unit.h"


//==========================================================
// - ����
HRESULT town::init(int xIndex, int yIndex, CountryColor::Enum color)
{
	terrainTile* tile = WORLD->getMap()->getTile(xIndex, yIndex);
	_height = tile->getHeight();
	tile->setObjectOnTile(this);

	gameObject::init("����", "town", tileMap::getTilePosFromIndex(vector2D(xIndex, yIndex), _height));
	_pos = _pos / CAMERA->getZoom();

	_frame = color;
	_index.x = xIndex;
	_index.y = yIndex;

	_hp = 50;
	_farmTimer = 0;
	_countryColor = color;

	return S_OK;
}

void town::release()
{
	terrainTile* tile = WORLD->getMap()->getTile(_index.x, _index.y);
	tile->removeObjectOnTile();

	gameObject::release();
}

void town::update()
{
	gameObject::update();

	//���� ����


	//���� ����
	_farmTimer += TIMEMANAGER->getElapsedTime();
	if (_farmTimer >= 3.0f)
	{
		// - 1. �ڱ� �ֺ� 8ĭ ������.
		terrainTile* tiles[8];
		WORLD->getMap()->get8Tiles(tiles, _index.x, _index.y);

		// - 2. �� Ÿ�Ͽ� ���� ����
		for (int i = 0; i < 8; ++i)
		{
			if (tiles[i])
			{
				//������Ʈ�� ���� Ÿ��
				if (!tiles[i]->getObjectOnTile())
				{
					farmLand* farm = new farmLand;
					farm->init(tiles[i]->getIndex().x, tiles[i]->getIndex().y, _countryColor);
					WORLD->addObject(farm);
					break;
				}
			}
		}

		_farmTimer -= 3.0f;
		_hp += 40;
	}

	//���� ����
	if (_hp > 100)
	{
		_hp -= 100;
		unit* noyae = new unit;
		noyae->init(_index, _height, _countryColor);
		WORLD->addUnit(noyae, _countryColor);
	}

}

void town::render()
{
	if (_image)
	{
		float zoom = CAMERA->getZoom();
		_image->setAlphaOption(_alpha);
		_image->setScaleOption(vector2D(zoom, zoom));
		_image->frameRender(_pos.x*zoom, _pos.y*zoom, _frame, 0, _pivot);
	}
}


//===============================================================
// - ����
HRESULT farmLand::init(int xIndex, int yIndex, CountryColor::Enum color)
{
	terrainTile* tile = WORLD->getMap()->getTile(xIndex, yIndex);
	_height = tile->getHeight();
	tile->setObjectOnTile(this);

	gameObject::init("����", "farmLand", tileMap::getTilePosFromIndex(vector2D(xIndex, yIndex), _height));
	_pos = _pos / CAMERA->getZoom();

	_index.x = xIndex;
	_index.y = yIndex;
	_frame = color;

	_hp = 30;

	return S_OK;
}

void farmLand::release()
{
	terrainTile* tile = WORLD->getMap()->getTile(_index.x, _index.y);
	tile->removeObjectOnTile();

	gameObject::release();
}

void farmLand::update()
{
	gameObject::update();

	//������ ���� �ֱ�


}

void farmLand::render()
{
	if (_image)
	{
		float zoom = CAMERA->getZoom();
		_image->setAlphaOption(_alpha);
		_image->setScaleOption(vector2D(zoom, zoom));
		_image->frameRender(_pos.x*zoom, _pos.y*zoom, _frame, 0, _pivot);
	}
}
