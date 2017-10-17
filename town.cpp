#include "stdafx.h"
#include "town.h"
#include "unit.h"

//==========================================================
// - 밸런스 설정값

#define FARM_CREATE_TIME 5.0f		//농장 생성 시간
#define PAY_TAX_TIME 5.0f			//세금 납부 시간

#define TOWN_INCOME_TIME 4.0f		//마을 수익 시간
#define FARM_INCOME_TIME 5.0f		//농장 수익 시간


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
	_taxTimer = 0;
	_townIncomeTimer = 0;

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

	//돈 먹기
	_townIncomeTimer += TIMEMANAGER->getElapsedTime();
	if (_townIncomeTimer >= TOWN_INCOME_TIME)
	{
		this->addHp(2);

		_townIncomeTimer -= TOWN_INCOME_TIME;
	}

	//세금 납부
	//세금 공식: 자기HP*세율/3;
	_taxTimer += TIMEMANAGER->getElapsedTime();
	if (_taxTimer >= PAY_TAX_TIME)
	{
		country* cty = WORLD->getCountry(_countryColor);
		float tax = _hp*cty->getTaxRate() / 3;	//세금 납부 공식
		int pay = (_hp - tax > 0) ? _hp - tax : _hp - 1;

		//세금 납부
		_hp -= pay;
		cty->addGold(pay);

		_taxTimer -= PAY_TAX_TIME;
	}

	//농장 생성
	_farmTimer += TIMEMANAGER->getElapsedTime();
	if (_farmTimer >= FARM_CREATE_TIME)
	{
		if (_hp > 30)
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

						_hp -= 30;
						break;
					}
				}
			}
		}

		_farmTimer -= FARM_CREATE_TIME;
	}

	//유닛 생성
	if (_hp > 100)
	{
		_hp -= 100;
		unit* noyae = new unit;
		noyae->init(_index, _height, _countryColor);
		WORLD->addUnit(noyae, _countryColor);
	}

	if (_hp <= 0)
		this->setDestroy();

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
	_incomeTimer = 0;

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
	_incomeTimer += TIMEMANAGER->getElapsedTime();
	if (_incomeTimer >= FARM_INCOME_TIME)
	{
		terrainTile* tiles[8];
		WORLD->getMap()->get8Tiles(tiles, _index.x, _index.y);

		//주변 8타일의 마을에게 수익 분배
		for (int i = 0; i < 8; ++i)
		{
			if (tiles[i])
			{
				gameObject* obj = tiles[i]->getObjectOnTile();
				if (obj)
				{
					if (obj->_name == "마을")
					{
						if (((town*)obj)->getCountryColor() == _countryColor)
						{
							((town*)obj)->addHp(1);
						}
					}
				}
			}
		}

		_incomeTimer -= FARM_INCOME_TIME;
	}

	if (_hp <= 0)
		this->setDestroy();
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
