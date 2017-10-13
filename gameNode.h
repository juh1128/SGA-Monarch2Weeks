#pragma once
#include "image.h"

class gameNode
{
private:
	bool _managerInit;

public:
	virtual HRESULT init(void);
	virtual HRESULT init(bool managerInit);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	LRESULT MainProc(HWND , UINT, WPARAM, LPARAM);

	gameNode();
	virtual ~gameNode();
};

