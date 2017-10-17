#pragma once
#include "mncObjectBase.h"


class bridge : public mncObjectBase
{
private:
	int						_frame;
public:
	bridge() {}
	virtual ~bridge() {}

	HRESULT init(int xIndex, int yIndex);
	void release();
	void update();
	void render();
};