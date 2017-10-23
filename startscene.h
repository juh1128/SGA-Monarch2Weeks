#pragma once
#include "sceneBase.h"
#include "testScene.h"

class startscene : public sceneBase 
{
	private:
		bool word; 
		bool click; 
		
		RECT rc_1; 
		RECT rc_2; 
		RECT rc_3; 

		float x,y; 
		

	public:
	virtual HRESULT init();
	virtual void release(); 
	virtual void resume();
	virtual void update();
	virtual void render(); 

	startscene();
	~startscene();
};

