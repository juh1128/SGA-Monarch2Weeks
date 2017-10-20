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
//	//�ֺ� 8���� Ÿ���� Ȯ���ؼ� ���ȭ��Ű�� ����Ʈ�� ��ȯ�Ѵ�.
	vector<vector2D> _passList;
	_passList.emplace_back(0, 0); //0,0�� ���� Ÿ���̹Ƿ� passList�� �߰��Ѵ�.

	//����, ���� 4������ ���� ��ġ��
	//�� ����� ���� ������ 4������ �� ��ģ��.
	auto add = [&](int i, int j)->bool
	{
		//�׳� �Ѿ���� üũ�Ѵ�.
		for (int cnt = 0; cnt < _passList.size(); ++cnt)
		{
			if (i == _passList[cnt].x && j == _passList[cnt].y)
				return false;
		}
		//���� ������ ���� �Ѿ�� continue;
		if (_currentNode->_tile->getIndex().x + j < 0 || _currentNode->_tile->getIndex().y + i < 0 ||
			_currentNode->_tile->getIndex().x + j >= WORLD->getMapSize().x ||
			_currentNode->_tile->getIndex().y + i >= WORLD->getMapSize().y)
			return false;

		terrainTile* tile = WORLD->getMap()->getTile(_currentNode->_tile->getIndex().x + j, _currentNode->_tile->getIndex().y + i);
		//�ش� Ÿ���� close����Ʈ�� �ִ��� Ȯ��
		for (int cnt = 0; cnt < _closeList.size(); ++cnt)
		{
			if (tile == _closeList[cnt]->_tile)
				return false;
		}
		//�ش� Ÿ���� �̹� open����Ʈ�� �ִ��� Ȯ��
		for (int cnt = 0; cnt < _openList.size(); ++cnt)
		{
			if (tile == _openList[cnt]->_tile)
			{
				return false;
			}
		}

		//�ش� Ÿ���� �̵��Ұ� Ÿ���� ���.
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
		//�̵� �Ұ� Ÿ�ϵ� �ƴϰ�, passList���� ���� Ÿ���� openList�� �߰��Ѵ�.
		_openList.push_back(getChildNode(_currentNode, tile));
	};

	

	add(- 1, 0); //��
	add(1, 0); //��
	add(0, - 1); //��
	add(0, 1); //��
	

}

deque<terrainTile*> pathFinder::getPath(terrainTile * start, terrainTile * dest)
{
	deque<terrainTile*> path;

	//������ Ÿ���� �̵� �Ұ����� Ÿ���̸� ��ã�� ����.
	if (!dest->isWalkable()) return path;
	if (start == dest) return path;

	_startTile = start;
	_destTile = dest;

	//�غ��۾�: openList�� ���������� �ִ´�.
	tagAstarNode* startNode = new tagAstarNode;
	startNode->_costFromStart = 0;
	startNode->_tile = start;
	startNode->_parentNode = NULL;
	startNode->_costToGoal = 0;
	startNode->_totalCost = 5000;

	_openList.push_back(startNode);
	_currentNode = startNode;

	//Astar ��ã�� ����
	while (true)
	{
		//��������Ʈ �� ���� ������ ��带 ã�Ƽ� ��������Ʈ���� ��������Ʈ�� �̵���Ų��.
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

		//currentNode�� ���������� Ȯ���Ѵ�.
		if (_currentNode->_tile == dest)
		{
			//�������� �����ߴٸ�, ������ Ÿ�� �Ž����ö󰡸鼭 ���� ���� ��θ� �籸���Ѵ�.
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

		//���ο� ��ġ�� �������� �ٽ� ��������Ʈ�� �����Ѵ�.
		addOpenList();

		if (_openList.size() <= 0)
		{
			//�� �� �ִ� ���� ����.
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
	//���� ��ϰ� ���� ��� �ʱ�ȭ
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
