#include "stdafx.h"
#include "unit.h"

void unit::moveAstar(int x, int y)
{
	deque<terrainTile*> path = PATHFINDER->getPath(WORLD->getMap()->getTile(_index.x, _index.y), WORLD->getMap()->getTile(x, y));

	for (size_t i = 0; i < path.size();++i)
	{
		reserveState(new unitOneStep(path[i]->getIndex().x,path[i]->getIndex().y));
	}
}
