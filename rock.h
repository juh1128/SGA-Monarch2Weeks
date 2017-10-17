#pragma once
#include "mncObjectBase.h"


class rock : public mncObjectBase
{
private:

public:
	rock() {}
	virtual ~rock() {}

	HRESULT init(int xIndex, int yIndex);
	void release();
	void update();
	void render();
};