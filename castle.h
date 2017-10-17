#pragma once
#include "gameObject.h"


class castle : public gameObject
{
private:

public:
	castle() {}
	virtual ~castle() {}

	HRESULT init();
	void release();
	void update();
	void render();
};