#include "stdafx.h"
#include "tree.h"

HRESULT tree::init(int xIndex, int yIndex)
{
	mncObjectBase::init("나무", "tree", xIndex, yIndex, 80, false);

	_frame = 0;
	return S_OK;
}

void tree::release()
{
	mncObjectBase::release();
}

void tree::update()
{
	mncObjectBase::update();
}

void tree::render()
{
	mncObjectBase::frameRender(_frame, 0);
}

HRESULT weed::init(int xIndex, int yIndex)
{
	mncObjectBase::init("잡초", "weed", xIndex, yIndex, 20, false);
	return S_OK;
}

void weed::release()
{
	mncObjectBase::release();
}

void weed::update()
{
	mncObjectBase::update();
}

void weed::render()
{
	mncObjectBase::render();
}
