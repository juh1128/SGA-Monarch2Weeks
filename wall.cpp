#include "stdafx.h"
#include "wall.h"

HRESULT wall::init(int xIndex, int yIndex, CountryColor::Enum color)
{
	mncObjectBase::init("��å", "wall", xIndex, yIndex, 200, false);
	_cost = 200;

	_frame = 2;
	_countryColor = color;
	return S_OK;
}

void wall::release()
{
	mncObjectBase::release();
}

void wall::update()
{
	mncObjectBase::update();

	//ü�¿� ���� ������ ����
	float hpRatio = (float)_hp / (float)_maxHp;
	if (hpRatio > 0.66f)
	{
		_frame = 0;
	}
	else if (hpRatio >= 0.33f)
	{
		_frame = 1;
	}
	else
	{
		_frame = 2;
	}
}

void wall::render()
{
	mncObjectBase::frameRender(_frame, 0);
}
