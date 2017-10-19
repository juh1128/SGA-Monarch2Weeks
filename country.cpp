#include "stdafx.h"
#include "country.h"
#include "unit.h"

void country::init(CountryColor::Enum color)
{
	_countryColor = color;
	_gold = 5000;
	_taxRate = 0.1f;
	_countryPower = 0;
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
			_unitList[i] = NULL;

			_unitList.erase(_unitList.begin() + i);
			--i;
		}
	}

}

void country::render()
{
	//* 17.19.15 country에서 렌더링하지 않고, 타일에서 자기 타일 위에 있는 유닛을 렌더링해준다. *//

	////유닛 리스트를 순회하면서 렌더링
	//for (size_t i = 0; i < _unitList.size(); ++i)
	//{
	//	if (_unitList[i]->isActiveObject())
	//	{
	//		_unitList[i]->render();
	//	}
	//}
}

void country::addUnit(unit * newUnit)
{
	_unitList.push_back(newUnit);
}
