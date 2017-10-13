#pragma once
#include "gameObject.h"
#include <vector>
//static vector2D up(0, -1);
//static vector2D down(0, 1);
//static vector2D right(1,0);
//static vector2D left(-1,0);


class unit : public gameObject
{
private:
	vector2D _index;
public:
	unit();
	~unit();

	HRESULT init();
	void release();
	void update();
	void render();

	void moveCallBack(POINT dest);
};

