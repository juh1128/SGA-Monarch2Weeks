#pragma once
#include "gameObject.h"


class rock : public gameObject
{
private:
	POINT			_index;
	int				_height;

public:
	rock() {}
	virtual ~rock() {}

	HRESULT init(int xIndex, int yIndex);
	void release();
	void update();
	void render();

	int getHp() { return 0; }
};