#include "stdafx.h"
#include "bridge.h"

HRESULT bridge::init(int xIndex, int yIndex, CountryColor::Enum color)
{
	mncObjectBase::init("�ٸ�", "bridge", xIndex, yIndex, 100, true);
	_cost = 2000;

	_frame = 0;
	_countryColor = color;

	//�ٸ� ���� üũ direction: 0�̸� ����, 1�̸� ������

	return S_OK;
}

void bridge::release()
{
	mncObjectBase::release();
}

void bridge::update()
{
	mncObjectBase::update();

	//ü�¿� ���� ������ ����
	float hpRatio = (float)_hp / (float)_maxHp;
	if (hpRatio > 0.2f)
	{
		_frame = 0;
	}
	else
	{
		_frame = 1;
	}
}

void bridge::render()
{
	mncObjectBase::frameRender(_direction+_frame, 0);
}
