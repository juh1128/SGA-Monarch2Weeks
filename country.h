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
	float						_taxRate; //¼¼À²

	vector<unit*>				_unitList; //À¯´Ö¸®½ºÆ®

public:
	country() {}
	~country() {}

	void init(CountryColor::Enum color);
	void release();
	void update();
	void render();

	void addUnit(unit* newUnit);
	vector<unit*>* getUnitList() { return &_unitList; }

	//- µ· °ü·Ã
	int getGold() { return _gold; }
	void addGold(int add) { _gold += add; }
	void setGold(int set) { _gold = set; }
	float getTaxRate() { return _taxRate; }
	void setTaxRate(float set) { _taxRate = set; }
};