#include "stdafx.h"
#include "unit.h"

void unit::moveAstar(int x, int y)
{
	_reservedState.clear();

	if (this->getCountryColor() != CountryColor::BLUE) return;
	
	deque<terrainTile*> path = PATHFINDER->getPath(WORLD->getMap()->getTile(_index.x, _index.y), WORLD->getMap()->getTile(x, y));
	
	//cout << "����� ������ blue�� astar��� path ũ�� : "<< path.size() << endl;

	for (size_t i = 0; i < path.size();++i)
	{
		reserveState(new unitOneStep(path[i]->getIndex().x,path[i]->getIndex().y));
	}
}
