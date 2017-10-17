#include "stdafx.h"
#include "wall.h"

HRESULT wall::init(int xIndex, int yIndex)
{
	mncObjectBase::init("∏Ò√•", "wall", xIndex, yIndex, 200, false);
	return S_OK;
}

void wall::release()
{
	mncObjectBase::release();
}

void wall::update()
{
	mncObjectBase::update();
}

void wall::render()
{
	mncObjectBase::frameRender(_frame, 0);
}
