#pragma once
#include "sceneBase.h"
#include "unit.h"

class testScene : public sceneBase
{
private:
	vector2D			_rbDownPos;

public:
	virtual HRESULT init();
	virtual void release();

	virtual void resume();

	virtual void update();
	virtual void render();
};