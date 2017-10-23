#include "stdafx.h"
#include "bridge.h"

HRESULT bridge::init(int xIndex, int yIndex, CountryColor::Enum color)
{
	mncObjectBase::init("´Ù¸®", "bridge", xIndex, yIndex, 100, true);
	_cost = 2000;

	_frame = 0;
	_countryColor = color;
	return S_OK;
}

void bridge::release()
{
	mncObjectBase::release();
}

void bridge::update()
{
	mncObjectBase::update();
}

void bridge::render()
{
	mncObjectBase::frameRender(_frame, 0);
}
