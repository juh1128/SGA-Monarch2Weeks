#pragma once
#include "gameObject.h"

class unit;

class userInterface : public gameObject
{
private:
	tileMap*			_map;
	unit*				_pickedUnit;
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
	//유닛 피킹
	void pickUnit();



	//피킹된 타일 위의 오브젝트 정보 표시
	void renderPickInfo();
};