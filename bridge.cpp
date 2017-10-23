#include "stdafx.h"
#include "bridge.h"
#include "unit.h"

HRESULT bridge::init(int xIndex, int yIndex)
{
	mncObjectBase::init("�ٸ�", "bridge", xIndex, yIndex, 100, true);
	_cost = 2000;

	_frame = 0;

	//�ٸ� ���� üũ direction: 0�̸� ����, 1�̸� ������
	unit testUnit;
	//����
	if (testUnit.isMoveable(PointMake(xIndex-1, yIndex)) && testUnit.isMoveable(PointMake(xIndex+1, yIndex)))
	{
		_direction = 0;
	}
	else if (testUnit.isMoveable(PointMake(xIndex, yIndex - 1)) && testUnit.isMoveable(PointMake(xIndex, yIndex + 1)))
	{
		_direction = 1;
	}

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
	if (hpRatio > 0.33f)
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
