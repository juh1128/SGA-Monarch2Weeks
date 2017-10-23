#include "stdafx.h"
#include "bridge.h"

HRESULT bridge::init(int xIndex, int yIndex, CountryColor::Enum color)
{
	mncObjectBase::init("다리", "bridge", xIndex, yIndex, 100, true);
	_cost = 2000;

	_frame = 0;
	_countryColor = color;

	//다리 방향 체크 direction: 0이면 왼쪽, 1이면 오른쪽

	return S_OK;
}

void bridge::release()
{
	mncObjectBase::release();
}

void bridge::update()
{
	mncObjectBase::update();

	//체력에 따라 프레임 설정
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
