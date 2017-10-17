#include "stdafx.h"
#include "bridge.h"

HRESULT bridge::init(int xIndex, int yIndex)
{
	mncObjectBase::init("´Ù¸®", "bridge", xIndex, yIndex, 100, true);

	_frame = 0;
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
