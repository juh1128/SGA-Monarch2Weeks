#pragma once
#include "gameObject.h"


class userInterface : public gameObject
{
private:
	tileMap*			_map;
	vector2D			_rbDownPos;

public:
	userInterface() {}
	virtual ~userInterface() {}

	HRESULT init();
	void release();
	void render();
	void update();

	//카메라 이동
	void moveCamera();
	//타일 피킹
	void pickingTile();

	//피킹된 타일 위의 오브젝트 정보 표시
	void renderPickInfo();
};