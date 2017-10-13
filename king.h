#pragma once
#include "gameObject.h"

class king : public gameObject
{
public:
	HRESULT init();
	void release();
	void update();
	void render();


	king();
	~king();
};

