#include "stdafx.h"
#include "unit.h"

void unitCreateMotion::enter(unit& me)
{
	_createFrameTimer = 0;
	_rotateNum = 0;
	me._pos.y += tileMap::getTileSize().y * 0.3f;
	me._state = UnitState::CreateMotion;
}

void unitCreateMotion::update(unit & me)
{
	//생성 연출
	if (_rotateNum < 8)
	{
		_createFrameTimer += TIMEMANAGER->getElapsedTime();
		if (_createFrameTimer >= 0.1f)
		{
			me._pos.y -= tileMap::getTileSize().y * 0.03f;
			_createFrameTimer -= 0.1f;
			int dir = me._unitDirection;
			dir = ++dir % 4;
			me._unitDirection = (UnitDirection::DIRECTION)dir;
			_rotateNum++;
		}
	}
	else
	{
		me.changeState(new unitNoneState);
	}
}
