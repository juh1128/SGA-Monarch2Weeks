#pragma once
#include "mncObjectBase.h"


class tree : public mncObjectBase
{
private:
	int				_frame;

public:
	tree() {}
	virtual ~tree() {}

	HRESULT init(int xIndex, int yIndex);
	void release();
	void update();
	void render();
};

class weed : public mncObjectBase
{
private:
	int				_hp;

public:
	weed() {}
	virtual ~weed() {}

	HRESULT init(int xIndex, int yIndex);
	void release();
	void update();
	void render();
};