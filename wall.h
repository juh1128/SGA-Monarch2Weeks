#pragma once
#include "gameObject.h"


class wall : public gameObject
{
private:
	POINT			_index;
	int				_height;
	int				_frame;

	int				_hp;

public:
	wall() {}
	virtual ~wall() {}

	HRESULT init(int xIndex, int yIndex);
	void release();
	void update();
	void render();
};