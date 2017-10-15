#pragma once
#include "gameObject.h"
#include "country.h"

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

