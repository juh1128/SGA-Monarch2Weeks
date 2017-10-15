#pragma once

class unit;

namespace CountryColor
{
	enum Enum
	{
		RED, WHITE, BLUE, GREEN, END
	};
}

class country
{
private:
	CountryColor::Enum			_countryColor;
	int							_gold;   //µ·
	float						_income; //¼¼À²

	vector<unit*>				_unitList; //À¯´Ö¸®½ºÆ®

public:
	country() {}
	~country() {}

	void init(CountryColor::Enum color, int startGold, float startIncome);
	void release();
	void update();
	void render();

	void addUnit(unit* newUnit);
	vector<unit*>* getUnitList() { return &_unitList; }
};