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
	if (!_isLive) return;

	_timer += TIMEMANAGER->getElapsedTime();

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

	//�ش� ������ ��� �ǹ��� ���� ����
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
			return "�޵�";
			break;
		case CountryColor::WHITE:
			return "�α׷�";
			break;
		case CountryColor::BLUE:
			return "��Ʈ����";
			break;
		case CountryColor::GREEN:
			return "�˺�";
			break;
		}
	};

	char buffer[128] = "";
	wsprintf(buffer, "%s ������ ����Ͽ���...", getCountryName(_countryColor));
	((testScene*)SCENEMANAGER->getNowScene())->getUI()->drawSystemText(buffer, 3.0f, RGB(0, 0, 0));
}