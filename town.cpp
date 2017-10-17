#include "stdafx.h"
#include "town.h"
#include "unit.h"

//==========================================================
// - �뷱�� ������

#define FARM_CREATE_TIME 5.0f		//���� ���� �ð�
#define PAY_TAX_TIME 5.0f			//���� ���� �ð�

#define TOWN_INCOME_TIME 4.0f		//���� ���� �ð�
#define FARM_INCOME_TIME 5.0f		//���� ���� �ð�


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

	//�� �Ա�
	_townIncomeTimer += TIMEMANAGER->getElapsedTime();
	if (_townIncomeTimer >= TOWN_INCOME_TIME)
	{
		this->addHp(2);

		_townIncomeTimer -= TOWN_INCOME_TIME;
	}

	//���� ����
	//���� ����: �ڱ�HP*����/3;
	_taxTimer += TIMEMANAGER->getElapsedTime();
	if (_taxTimer >= PAY_TAX_TIME)
	{
		country* cty = WORLD->getCountry(_countryColor);
		float tax = _hp*cty->getTaxRate() / 3;	//���� ���� ����
		int pay = (_hp - tax > 0) ? _hp - tax : _hp - 1;

		//���� ����
		_hp -= pay;
		cty->addGold(pay);

		_taxTimer -= PAY_TAX_TIME;
	}

	//���� ����
	_farmTimer += TIMEMANAGER->getElapsedTime();
	if (_farmTimer >= FARM_CREATE_TIME)
	{
		if (_hp > 30)
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

						_hp -= 30;
						break;
					}
				}
			}
		}

		_farmTimer -= FARM_CREATE_TIME;
	}

	//���� ����
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

	//������ ���� �ֱ�
	_incomeTimer += TIMEMANAGER->getElapsedTime();
	if (_incomeTimer >= FARM_INCOME_TIME)
	{
		terrainTile* tiles[8];
		WORLD->getMap()->get8Tiles(tiles, _index.x, _index.y);

		//�ֺ� 8Ÿ���� �������� ���� �й�
		for (int i = 0; i < 8; ++i)
		{
			if (tiles[i])
			{
				gameObject* obj = tiles[i]->getObjectOnTile();
				if (obj)
				{
					if (obj->_name == "����")
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
