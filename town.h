#pragma once
#include "gameObject.h"


class farmLand : public gameObject
{
private:

public:
	farmLand() {}
	virtual ~farmLand() {}

	HRESULT init();
	void release();
	void update();
	void render();
};

class town : public gameObject
{
private:

public:
	town() {}
	virtual ~town() {}

	HRESULT init();
	void release();
	void update();
	void render();
};