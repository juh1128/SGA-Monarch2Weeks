#include "stdafx.h"
#include "unit.h"

void unitOneStep::unitdirection(unit & unit)
{	
	//UP
	if (unit._index.x == _directionIndex.x
		&& unit._index.y > _directionIndex.y)
	{
		unit._unitDirection = UnitDirection::UNIT_UP;
	}
	//DOWN
	else if (unit._index.x == _directionIndex.x
		&& unit._index.y < _directionIndex.y)
	{
		unit._unitDirection = UnitDirection::UNIT_DOWN;
	}
	//LFET
	else if (unit._index.x > _directionIndex.x
		&& unit._index.y == _directionIndex.y)
	{
		unit._unitDirection = UnitDirection::UNIT_LEFT;
	}
	//RIGHT
	else if (unit._index.x < _directionIndex.x
		&& unit._index.y == _directionIndex.y)
	{
		unit._unitDirection = UnitDirection::UNIT_RIGHT;
	}
}