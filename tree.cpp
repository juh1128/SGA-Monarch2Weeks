#include "stdafx.h"
#include "tree.h"

HRESULT tree::init(int xIndex, int yIndex)
{
	mncObjectBase::init("����", "tree", xIndex, yIndex, 50, false);

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

void tree::render()
{
	mncObjectBase::frameRender(_frame, 0);
}

HRESULT weed::init(int xIndex, int yIndex)
{
	mncObjectBase::init("����", "weed", xIndex, yIndex, 20, false);
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
