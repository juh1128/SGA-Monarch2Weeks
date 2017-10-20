#include "stdafx.h"
#include "town.h"
#include "unit.h"

//==========================================================
// - 밸런스 설정값

#define FARM_CREATE_TIME 3.0f		//농장 생성 시간
#define PAY_TAX_TIME 1.5f			//세금 납부 시간

#define TOWN_INCOME_TIME 1.0f		//마을 수익 시간
#define FARM_INCOME_TIME 2.0f		//농장 수익 시간


//==========================================================
// - 마을
HRESULT town::init(int xIndex, int yIndex, CountryColor::Enum color)
{
	mncObjectBase::init("마을", "town", xIndex, yIndex, 50, true);
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
		float tax = (_hp*cty->getTaxRate()) / 3;	//세금 납부 공식
		int pay = (_hp - tax > 0) ? tax : _hp - 1;

		//세금 납부
		_hp -= pay;
		cty->addGold(pay);

		_taxTimer -= PAY_TAX_TIME;
	}

	//농장 생성
	_farmTimer += TIMEMANAGER->getElapsedTime();
	if (_farmTimer >= FARM_CREATE_TIME)
	{
		if (_hp > 10)
		{
			// - 1. 자기 주변 8칸 가져옴.
			terrainTile* tiles[8];
			WORLD->getMap()->get8Tiles(tiles, _index.x, _index.y);

			// - 2. 빈 타일에 농장 생성
			for (int i = 0; i < 8; ++i)
			{
				if (tiles[i])
				{
					//건설 가능한지 확인
					if (tiles[i]->isBuildable())
					{
						//높이 차이 체크
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

	//유닛 생성
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
// - 농장
HRESULT farmLand::init(int xIndex, int yIndex, CountryColor::Enum color)
{
	mncObjectBase::init("농장", "farmLand", xIndex, yIndex, 10, true);

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
	mncObjectBase::frameRender(_frame, 0);
}
