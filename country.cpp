#include "stdafx.h"
#include "country.h"
#include "unit.h"

void country::init(CountryColor::Enum color, int startGold, float startIncome)
{
	_countryColor = color;
	_gold = startGold;
	_income = startIncome;
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
	//유닛 리스트를 순회하면서 업데이트
	for (size_t i = 0; i < _unitList.size(); ++i)
	{
		if (_unitList[i]->isActiveObject())
		{
			_unitList[i]->update();
		}
		//유닛이 죽었으면 제거해준다.
		else if (!_unitList[i]->isLive())
		{
			_unitList[i]->release();
			delete _unitList[i];
			_unitList[i] = NULL;

			_unitList.erase(_unitList.begin() + i);
			--i;
		}
	}

}

void country::render()
{
	//유닛 리스트를 순회하면서 렌더링
	for (size_t i = 0; i < _unitList.size(); ++i)
	{
		if (_unitList[i]->isActiveObject())
		{
			_unitList[i]->render();
		}
	}
}

void country::addUnit(unit * newUnit)
{
	_unitList.push_back(newUnit);
}
