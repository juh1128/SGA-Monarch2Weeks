#pragma once
#include "gameObject.h"


class rock : public gameObject
{
private:

public:
	rock() {}
	virtual ~rock() {}

	HRESULT init();
	void release();
	void update();
	void render();
};