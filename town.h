#pragma once
#include "mncObjectBase.h"

class farmLand : public mncObjectBase
{
private:
	int				_frame;
	float				_incomeTimer;

public:
	farmLand() {}
	virtual ~farmLand() {}

	HRESULT init(int xIndex, int yIndex, CountryColor::Enum color);
	void release();
	void update();
	void render();
};

class town : public mncObjectBase
{
private:
	int				_frame;

	float			_farmTimer;
	float			_taxTimer;
	float			_townIncomeTimer;

public:
	town() {}
	virtual ~town() {}

	HRESULT init(int xIndex, int yIndex, CountryColor::Enum color);
	void release();
	void update();
	void render();
};