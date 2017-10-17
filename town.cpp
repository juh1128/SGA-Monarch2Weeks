#include "stdafx.h"
#include "town.h"
#include "unit.h"


//==========================================================
// - 마을
HRESULT town::init(int xIndex, int yIndex, CountryColor::Enum color)
{
	terrainTile* tile = WORLD->getMap()->getTile(xIndex, yIndex);
	_height = tile->getHeight();
	tile->setObjectOnTile(this);

	gameObject::init("마을", "town", tileMap::getTilePosFromIndex(vector2D(xIndex, yIndex), _height));
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

	//세금 납부


	//농장 생성
	_farmTimer += TIMEMANAGER->getElapsedTime();
	if (_farmTimer >= 3.0f)
	{
		// - 1. 자기 주변 8칸 가져옴.
		terrainTile* tiles[8];
		WORLD->getMap()->get8Tiles(tiles, _index.x, _index.y);

		// - 2. 빈 타일에 농장 생성
		for (int i = 0; i < 8; ++i)
		{
			if (tiles[i])
			{
				//오브젝트가 없는 타일
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

	//유닛 생성
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
// - 농장
HRESULT farmLand::init(int xIndex, int yIndex, CountryColor::Enum color)
{
	terrainTile* tile = WORLD->getMap()->getTile(xIndex, yIndex);
	_height = tile->getHeight();
	tile->setObjectOnTile(this);

	gameObject::init("농장", "farmLand", tileMap::getTilePosFromIndex(vector2D(xIndex, yIndex), _height));
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

	//마을에 수익 주기


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
