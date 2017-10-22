#include "stdafx.h"
#include "country.h"
#include "unit.h"
#include "testScene.h"
#include "userInterface.h"
#include "mncObjectBase.h"

void country::init(CountryColor::Enum color)
{
	_countryColor = color;
	_gold = 5000;
	_taxRate = 0.1f;
	_countryPower = 0;
	_townCount = 0;
	_timer = 0;

	_isLive = true;
}

void country::release()
{
	//유닛 해제
	for (size_t i = 0; i < _unitList.size(); ++i)
	{
		_unitList[i]->release();
		delete _unitList[i];
		_unitList[i] = NULL;
	}
	_unitList.clear();
}

void country::update()
{
	if (!_isLive) return;

	_timer += TIMEMANAGER->getElapsedTime();

	//유닛 리스트를 순회하면서 업데이트
	_countryPower = 0;
	for (size_t i = 0; i < _unitList.size(); ++i)
	{
		if (_unitList[i]->isActiveObject())
		{
			_unitList[i]->update();
			_countryPower += _unitList[i]->getHealth();
		}
		//유닛이 죽었으면 제거해준다.
		else if (!_unitList[i]->isLive())
		{
			_unitList[i]->release();
			delete _unitList[i];
			_unitList[i]->setDestroy();
			_unitList[i] = NULL;

			_unitList.erase(_unitList.begin() + i);
			--i;
		}
	}

}

void country::render()
{
}

void country::addUnit(unit * newUnit)
{
	_unitList.push_back(newUnit);
}


void country::gameOver()
{
	if (!_isLive) return;


	_isLive = false;

	//해당 국가의 모든 건물과 유닛 제거
	for (size_t i = 0; i < _unitList.size(); ++i)
	{
		_unitList[i]->release();
		delete _unitList[i];
		_unitList[i]->setDestroy();
		_unitList[i] = NULL;
	}
	_unitList.clear();

	WORLD->destroyCountryBuilding(_countryColor);

	auto getCountryName = [](CountryColor::Enum color) {
		switch (color)
		{
		case CountryColor::RED:
			return "휴디";
			break;
		case CountryColor::WHITE:
			return "로그로";
			break;
		case CountryColor::BLUE:
			return "배트랜드";
			break;
		case CountryColor::GREEN:
			return "알빈";
			break;
		}
	};

	char buffer[128] = "";
	wsprintf(buffer, "%s 국가가 멸망하였다...", getCountryName(_countryColor));
	((testScene*)SCENEMANAGER->getNowScene())->getUI()->drawSystemText(buffer, 3.0f, RGB(0, 0, 0));
}