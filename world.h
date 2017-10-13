#pragma once
#include "tileMap.h"

#define MAX_LAYER 5

//씬 내에서 오브젝트들을 총괄하는 클래스다.
class world
{
private:
	vector<gameObject*> _objectList[MAX_LAYER];

	//타일맵 정보
	tileMap*			_tileMap;

public:
	world() {}
	~world() {}

	void init();
	void update();
	void render();
	void release();

	//오브젝트를 추가한다. (2번째 인자는 렌더링 우선순위)
	void addObject(gameObject* obj, int renderPriority = 1);
	//name이 일치하는 오브젝트를 찾아서 반환한다.
	gameObject* findObject(string name);
	//name이 일치하는 오브젝트를 모두 찾아서 벡터로 반환한다.
	vector<gameObject*> findObjects(string name);
	
	vector<gameObject*> findObjects(string name, int priorityNum);


	//===============================================
	// ## 타일맵 관련 ##
	//===============================================

	//타일 모두 해제
	void releaseTiles();
	//타일 생성
	void loadTiles(string directory);
	//맵 사이즈
	vector2D getMapSize();
	//타일의 개수
	POINT getTileCount();

	//타일 리스트 접근 (해당 인덱스 타일의 이동 가능 여부를 반환한다.)
	bool isPossibleMove(int xIndex, int yIndex);
	//타일 리스트 접근 (해당 인덱스 타일의 이동속도값을 반환한다.)
	float getTileMoveRatio(int xIndex, int yIndex);
	
	//타일맵 반환
	tileMap* getMap() { return _tileMap; }

};