#pragma once
#include "mncObjectBase.h"


class wall : public mncObjectBase
{
private:
	int				_frame;

public:
	wall() {}
	virtual ~wall() {}

	HRESULT init(int xIndex, int yIndex, CountryColor::Enum color);
	void release();
	void update();
	void render();
};