#pragma once
#include "singletonBase.h"
#include <deque>


class pathFinder : public singletonBase<pathFinder>
{
private:
	struct tagAstarNode		//���
	{
		terrainTile*	_tile;
		int				_totalCost;
		int				_costFromStart;
		int				_costToGoal;

		//���� ���
		tagAstarNode*	_parentNode;
	};

private:
	terrainTile*		_startTile;
	terrainTile*		_destTile;

	tagAstarNode*				_currentNode;

	vector<tagAstarNode*>		_openList;  //������ ������ �ִ� �͵�
	vector<tagAstarNode*>		_closeList; //���� ��

public:
	pathFinder() {}
	virtual ~pathFinder() {}

	void init();
	void release();

	//Astar �˰������� ���� ���� ���� ã�Ƽ� ��ȯ���ش�.
	deque<terrainTile*>	getPath(terrainTile* start, terrainTile* dest);

	//����� ��忡�� ���
	void render();

private:
	//���� ����� �ֺ� 8������ ���鼭 Ÿ�ϵ��� ���ȭ��Ű��, ���¸���Ʈ�� �߰��Ѵ�.
	void addOpenList();

	//�ش� Ÿ���� ���ȭ��Ų��.
	tagAstarNode* getChildNode(tagAstarNode* parentNode, terrainTile* childTile);

	//����Ʈ �ʱ�ȭ
	void releaseList();
};