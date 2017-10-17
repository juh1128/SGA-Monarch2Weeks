#pragma once
#include "gameObject.h"


class tree : public gameObject
{
private:

public:
	tree() {}
	virtual ~tree() {}

	HRESULT init();
	void release();
	void update();
	void render();
};

class weed : public gameObject
{
private:

public:
	weed() {}
	virtual ~weed() {}

	HRESULT init();
	void release();
	void update();
	void render();
};