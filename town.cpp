#include "stdafx.h"
#include "town.h"
#include "unit.h"

//==========================================================
// - �뷱�� ������

#define FARM_CREATE_TIME 3.0f		//���� ���� �ð�
#define PAY_TAX_TIME 1.5f			//���� ���� �ð�

#define TOWN_INCOME_TIME 1.0f		//���� ���� �ð�
#define FARM_INCOME_TIME 2.0f		//���� ���� �ð�


//==========================================================
// - ����
HRESULT town::init(int xIndex, int yIndex, CountryColor::Enum color)
{
	mncObjectBase::init("����", "town", xIndex, yIndex, 50, true);
	_frame = color;

	_hp = 50;
	_farmTimer = 0;
	_taxTimer = 0;
	_townIncomeTimer = 0;

	_countryColor = color;

	return S_OK;
}

void town::release()
{
	mncObjectBase::release();
}

void town::update()
{
	mncObjectBase::update();

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
		float tax = (_hp*cty->getTaxRate()) / 3;	//���� ���� ����
		int pay = (_hp - tax > 0) ? tax : _hp - 1;

		//���� ����
		_hp -= pay;
		cty->addGold(pay);

		_taxTimer -= PAY_TAX_TIME;
	}

	//���� ����
	_farmTimer += TIMEMANAGER->getElapsedTime();
	if (_farmTimer >= FARM_CREATE_TIME)
	{
		if (_hp > 10)
		{
			// - 1. �ڱ� �ֺ� 8ĭ ������.
			terrainTile* tiles[8];
			WORLD->getMap()->get8Tiles(tiles, _index.x, _index.y);

			// - 2. �� Ÿ�Ͽ� ���� ����
			for (int i = 0; i < 8; ++i)
			{
				if (tiles[i])
				{
					//�Ǽ� �������� Ȯ��
					if (tiles[i]->isBuildable())
					{
						//���� ���� üũ
						if (abs(tiles[i]->getHeight()-_height) < 2)
						{
							farmLand* farm = new farmLand;
							farm->init(tiles[i]->getIndex().x, tiles[i]->getIndex().y, _countryColor);
							WORLD->addObject(farm);

							_hp -= 10;
						}
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
		noyae->init(_index, _height+1, _countryColor);
		WORLD->addUnit(noyae, _countryColor);
	}

	if (_hp <= 0)
		this->setDestroy();

}

void town::render()
{
	mncObjectBase::frameRender(_frame, 0);
}


//===============================================================
// - ����
HRESULT farmLand::init(int xIndex, int yIndex, CountryColor::Enum color)
{
	mncObjectBase::init("����", "farmLand", xIndex, yIndex, 10, true);

	_frame = color;
	_countryColor = color;
	_incomeTimer = 0;

	return S_OK;
}

void farmLand::release()
{
	mncObjectBase::release();
}

void farmLand::update()
{
	mncObjectBase::update();

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
	mncObjectBase::frameRender(_frame, 0);
}
