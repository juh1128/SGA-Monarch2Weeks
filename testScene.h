#pragma once
#include "unit.h"

class testScene : public sceneBase
{
private:
	unit* _unitOne;

public:
	virtual HRESULT init();
	virtual void release();

	virtual void resume();

	virtual void update();
	virtual void render();
};