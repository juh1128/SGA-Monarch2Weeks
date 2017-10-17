#pragma once
#include "gameObject.h"


class bridge : public gameObject
{
private:

public:
	bridge() {}
	virtual ~bridge() {}

	HRESULT init();
	void release();
	void update();
	void render();
};