#pragma once
#include "gameObject.h"


class unit : public gameObject
{
private:
public:
	unit();
	~unit();

	HRESULT init();
	void release();
	void update();
	void render();
};

