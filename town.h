#pragma once
#include "mncObjectBase.h"

class farmLand : public mncObjectBase
{
private:
	int				_frame;
	CountryColor::Enum	_countryColor;
	float				_incomeTimer;

public:
	farmLand() {}
	virtual ~farmLand() {}

	HRESULT init(int xIndex, int yIndex, CountryColor::Enum color);
	void release();
	void update();
	void render();

	CountryColor::Enum getCountryColor() { return _countryColor; }
};

class town : public mncObjectBase
{
private:
	int				_frame;
	CountryColor::Enum	_countryColor;

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

	CountryColor::Enum getCountryColor() { return _countryColor; }
};