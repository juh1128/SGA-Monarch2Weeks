#pragma once
#include "singletonBase.h"
#include <deque>


class pathFinder : public singletonBase<pathFinder>
{
private:
	struct tagAstarNode		//노드
	{
		terrainTile*	_tile;
		int				_totalCost;
		int				_costFromStart;
		int				_costToGoal;

		//이전 노드
		tagAstarNode*	_parentNode;
	};

private:
	terrainTile*		_startTile;
	terrainTile*		_destTile;

	tagAstarNode*				_currentNode;

	vector<tagAstarNode*>		_openList;  //조사의 여지가 있는 것들
	vector<tagAstarNode*>		_closeList; //조사 끝

public:
	pathFinder() {}
	virtual ~pathFinder() {}

	void init();
	void release();

	//Astar 알고리즘으로 가장 빠른 길을 찾아서 반환해준다.
	deque<terrainTile*>	getPath(terrainTile* start, terrainTile* dest);

	//디버그 모드에서 사용
	void render();

private:
	//현재 노드의 주변 8방향을 돌면서 타일들을 노드화시키고, 오픈리스트에 추가한다.
	void addOpenList();

	//해당 타일을 노드화시킨다.
	tagAstarNode* getChildNode(tagAstarNode* parentNode, terrainTile* childTile);

	//리스트 초기화
	void releaseList();
};