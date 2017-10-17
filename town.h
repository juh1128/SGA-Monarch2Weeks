#pragma once
#include "gameObject.h"

class farmLand : public gameObject
{
private:
	POINT			_index;
	int				_height;
	int				_frame;

	int				_hp;
	CountryColor::Enum	_countryColor;

public:
	farmLand() {}
	virtual ~farmLand() {}

	HRESULT init(int xIndex, int yIndex, CountryColor::Enum color);
	void release();
	void update();
	void render();
};

class town : public gameObject
{
private:
	POINT			_index;
	int				_height;
	int				_frame;

	int					_hp;
	CountryColor::Enum	_countryColor;

	float			_farmTimer;

public:
	town() {}
	virtual ~town() {}

	HRESULT init(int xIndex, int yIndex, CountryColor::Enum color);
	void release();
	void update();
	void render();
};