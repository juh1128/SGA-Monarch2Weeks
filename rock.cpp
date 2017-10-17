#include "stdafx.h"
#include "rock.h"

HRESULT rock::init(int xIndex, int yIndex)
{
	mncObjectBase::init("µ¹", "rock", xIndex, yIndex, 0, false);
	return S_OK;
}

void rock::release()
{
	mncObjectBase::release();
}

void rock::update()
{
	mncObjectBase::update();
}

void rock::render()
{
	mncObjectBase::render();
}
