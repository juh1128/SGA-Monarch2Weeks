#pragma once
#include "sceneBase.h"
#include "unit.h"

class userInterface;

class testScene : public sceneBase
{
private:
	userInterface* _ui;

public:
	virtual HRESULT init();
	virtual void release();

	virtual void resume();

	virtual void update();
	virtual void render();
};