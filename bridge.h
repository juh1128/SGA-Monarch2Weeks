#pragma once
#include "mncObjectBase.h"


class bridge : public mncObjectBase
{
private:
	int				_frame;
	int				_direction;
public:
	bridge() {}
	virtual ~bridge() {}

	HRESULT init(int xIndex, int yIndex, CountryColor::Enum color);
	void release();
	void update();
	void render();
};