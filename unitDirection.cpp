#include "stdafx.h"
#include "unit.h"

void unitOneStep::unitdirection(unit & unit)
{	
	//UP
	if (unit._index.x == _directionIndex.x
		&& unit._index.y > _directionIndex.y)
	{
		unit._imageFrameY = 1;
	}
	//DOWN
	else if (unit._index.x == _directionIndex.x
		&& unit._index.y < _directionIndex.y)
	{
		unit._imageFrameY = 3;
	}
	//LFET
	else if (unit._index.x < _directionIndex.x
		&& unit._index.y == _directionIndex.y)
	{
		unit._imageFrameY = 2;
	}
	//RIGHT
	else if (unit._index.x > _directionIndex.x
		&& unit._index.y == _directionIndex.y)
	{
		unit._imageFrameY = 0;
	}
}