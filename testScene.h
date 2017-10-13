#pragma once

class testScene : public sceneBase
{
private:

public:
	virtual HRESULT init();
	virtual void release();

	virtual void resume();

	virtual void update();
	virtual void render();
};