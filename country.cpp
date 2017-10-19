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
	//���� ����
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
	//���� ����Ʈ�� ��ȸ�ϸ鼭 ������Ʈ
	_countryPower = 0;
	for (size_t i = 0; i < _unitList.size(); ++i)
	{
		if (_unitList[i]->isActiveObject())
		{
			_unitList[i]->update();
			_countryPower += _unitList[i]->getHealth();
		}
		//������ �׾����� �������ش�.
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
	//* 17.19.15 country���� ���������� �ʰ�, Ÿ�Ͽ��� �ڱ� Ÿ�� ���� �ִ� ������ ���������ش�. *//

	////���� ����Ʈ�� ��ȸ�ϸ鼭 ������
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
