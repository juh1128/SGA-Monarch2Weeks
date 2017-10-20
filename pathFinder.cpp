#include "stdafx.h"
#include "pathFinder.h"




void pathFinder::init()
{
	_startTile = NULL;
	_destTile = NULL;
}

void pathFinder::release()
{
}

void pathFinder::render()
{
}



void pathFinder::addOpenList()
{
//	//주변 8방향 타일을 확인해서 노드화시키고 리스트를 반환한다.
	vector<vector2D> _passList;
	_passList.emplace_back(0, 0); //0,0은 현재 타일이므로 passList에 추가한다.

	//가로, 세로 4방향을 먼저 펼치고
	//그 결과에 따라 나머지 4방향을 더 펼친다.
	auto add = [&](int i, int j)->bool
	{
		//그냥 넘어갈건지 체크한다.
		for (int cnt = 0; cnt < _passList.size(); ++cnt)
		{
			if (i == _passList[cnt].x && j == _passList[cnt].y)
				return false;
		}
		//조사 범위가 맵을 넘어가면 continue;
		if (_currentNode->_tile->getIndex().x + j < 0 || _currentNode->_tile->getIndex().y + i < 0 ||
			_currentNode->_tile->getIndex().x + j >= WORLD->getMapSize().x ||
			_currentNode->_tile->getIndex().y + i >= WORLD->getMapSize().y)
			return false;

		terrainTile* tile = WORLD->getMap()->getTile(_currentNode->_tile->getIndex().x + j, _currentNode->_tile->getIndex().y + i);
		//해당 타일이 close리스트에 있는지 확인
		for (int cnt = 0; cnt < _closeList.size(); ++cnt)
		{
			if (tile == _closeList[cnt]->_tile)
				return false;
		}
		//해당 타일이 이미 open리스트에 있는지 확인
		for (int cnt = 0; cnt < _openList.size(); ++cnt)
		{
			if (tile == _openList[cnt]->_tile)
			{
				return false;
			}
		}

		//해당 타일이 이동불가 타일일 경우.
		if (!tile->isWalkable())
		{
			return false;
		}
		vector2D currentindex = _currentNode->_tile->getIndex();
		vector2D temp(i, j);
		vector2D direct = currentindex + temp;

		if (abs(WORLD->getMap()->getTile(currentindex.x, currentindex.y)->getHeight() - WORLD->getMap()->getTile(direct.x, direct.y)->getHeight() )> 1)
		{
			return false;
		}
		//이동 불가 타일도 아니고, passList에도 없는 타일은 openList에 추가한다.
		_openList.push_back(getChildNode(_currentNode, tile));
	};

	

	add(- 1, 0); //좌
	add(1, 0); //우
	add(0, - 1); //상
	add(0, 1); //하
	

}

deque<terrainTile*> pathFinder::getPath(terrainTile * start, terrainTile * dest)
{
	deque<terrainTile*> path;

	//목적지 타일이 이동 불가능한 타일이면 길찾기 안함.
	if (!dest->isWalkable()) return path;
	if (start == dest) return path;

	_startTile = start;
	_destTile = dest;

	//준비작업: openList에 시작지점을 넣는다.
	tagAstarNode* startNode = new tagAstarNode;
	startNode->_costFromStart = 0;
	startNode->_tile = start;
	startNode->_parentNode = NULL;
	startNode->_costToGoal = 0;
	startNode->_totalCost = 5000;

	_openList.push_back(startNode);
	_currentNode = startNode;

	//Astar 길찾기 시작
	while (true)
	{
		//열린리스트 중 가장 저렴한 노드를 찾아서 열린리스트에서 닫힌리스트로 이동시킨다.
		int cheapIndex = 0;
		for (size_t i = 1; i < _openList.size(); ++i)
		{
			if (_openList[cheapIndex]->_totalCost > _openList[i]->_totalCost)
			{
				cheapIndex = i;
			}
		}
		_currentNode = _openList[cheapIndex];
		_closeList.push_back(_openList[cheapIndex]);
		_openList.erase(_openList.begin() + cheapIndex);

		//currentNode가 목적지인지 확인한다.
		if (_currentNode->_tile == dest)
		{
			//목적지에 도착했다면, 노드들을 타고 거슬러올라가면서 가장 빠른 경로를 재구성한다.
			tagAstarNode* parentNode = _currentNode->_parentNode;
			path.push_front(_currentNode->_tile);

			while (parentNode != startNode)
			{
				path.push_front(parentNode->_tile);
				parentNode = parentNode->_parentNode;
			}
			releaseList();
			return std::move(path);
		}

		//새로운 위치를 기준으로 다시 열린리스트를 갱신한다.
		addOpenList();

		if (_openList.size() <= 0)
		{
			//갈 수 있는 길이 없음.
			break;
		}
	}

	releaseList();
	return std::move(path);
}



pathFinder::tagAstarNode* pathFinder::getChildNode(tagAstarNode* parentNode, terrainTile * childTile)
{
	tagAstarNode* node = new tagAstarNode;
	node->_parentNode = parentNode;
	node->_tile = childTile;

	if (abs(childTile->getIndex().x - parentNode->_tile->getIndex().x) +
		abs(childTile->getIndex().y - parentNode->_tile->getIndex().y) == 1)
	{
		node->_costFromStart = parentNode->_costFromStart + 10;
	}
	else if(abs(childTile->getIndex().x - parentNode->_tile->getIndex().x) + 
			abs(childTile->getIndex().y - parentNode->_tile->getIndex().y) == 1 && 
			abs(childTile->getHeight() - parentNode->_tile->getHeight() )== 1)
	{
		node->_costFromStart = parentNode->_costFromStart + 16;
	}

	node->_costToGoal = (abs(_destTile->getIndex().x - childTile->getIndex().x) +
		abs(_destTile->getIndex().y - childTile->getIndex().y))*10;

	node->_totalCost = node->_costFromStart + node->_costToGoal;

	return node;

	return NULL;
}

void pathFinder::releaseList()
{
	//열린 목록과 닫힌 목록 초기화
	for (int i = _openList.size() - 1; i >= 0; --i)
	{
		delete _openList[i];
		_openList[i] = NULL;
		_openList.pop_back();
	}
	for (int i = _closeList.size() - 1; i >= 0; --i)
	{
		delete _closeList[i];
		_closeList[i] = NULL;
		_closeList.pop_back();
	}
}
