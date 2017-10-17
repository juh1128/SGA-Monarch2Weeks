#pragma once
#include "gameObject.h"

class farmLand : public gameObject
{
private:
	POINT			_index;
	int				_height;
	int				_frame;

	int					_hp;
	CountryColor::Enum	_countryColor;

	float				_incomeTimer;

public:
	farmLand() {}
	virtual ~farmLand() {}

	HRESULT init(int xIndex, int yIndex, CountryColor::Enum color);
	void release();
	void update();
	void render();

	int getHp() { return _hp; }
	void addHp(int add) { _hp += add; }
	void setHp(int set) { _hp = set; }

	CountryColor::Enum getCountryColor() { return _countryColor; }
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
	float			_taxTimer;
	float			_townIncomeTimer;

public:
	town() {}
	virtual ~town() {}

	HRESULT init(int xIndex, int yIndex, CountryColor::Enum color);
	void release();
	void update();
	void render();

	int getHp() { return _hp; }
	void addHp(int add) { _hp += add; }
	void setHp(int set) { _hp = set; }

	CountryColor::Enum getCountryColor() { return _countryColor; }
};