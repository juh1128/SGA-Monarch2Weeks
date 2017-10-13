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
	int							_gold;
	float						_income;

	vector<unit*>				_unitList;

public:
	country() {}
	~country() {}

	void init(CountryColor::Enum color, int startGold, float startIncome);
	void release();
	void update();
	void render();

	void addUnit(unit* newUnit);
};