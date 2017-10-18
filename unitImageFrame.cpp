#include "stdafx.h"
#include "unit.h"

void unit::imageFrame(void)
{
	_frameTimer += TIMEMANAGER->getElapsedTime();
	if (_frameTimer >= 0.4f)
	{
		_frameTimer -= 0.4f;
		_imageFrameX = ++_imageFrameX % 2;
	}
}

