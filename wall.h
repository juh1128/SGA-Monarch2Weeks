#pragma once
#include "gameObject.h"


class wall : public gameObject
{
private:

public:
	wall() {}
	virtual ~wall() {}

	HRESULT init();
	void release();
	void update();
	void render();
};