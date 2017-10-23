#pragma once

class unit;

namespace CountryColor
{
	enum Enum
	{
		RED, WHITE, BLUE, GREEN, NATURE, END
	};
}

class country
{
private:
	CountryColor::Enum			_countryColor;
	int							_gold;   //돈
	float						_taxRate; //세율

	vector<unit*>				_unitList; //유닛리스트
	int							_countryPower; //전투력

	bool						_isLive;

	float						_timer;
	int							_townCount;

	int							_frameTimer;

public:
	country() {}
	~country() {}

	void init(CountryColor::Enum color);
	void release();
	void update();
	void render();

	void addUnit(unit* newUnit);
	vector<unit*>* getUnitList() { return &_unitList; }

	//- 돈 관련
	int getGold() { return _gold; }
	void addGold(int add) { _gold += add; }
	void setGold(int set) { _gold = set; }
	float getTaxRate() { return _taxRate; }
	void setTaxRate(float set) { _taxRate = set; }

	//- 기타
	int getCountryPower() { return _countryPower; }
	CountryColor::Enum getColor() { return _countryColor; }
	void addTown() { _townCount++; }
	void removeTown() 
	{ 
		_townCount--;
		if (_townCount <= 0 && _timer >= 10.0f)
		{
			gameOver();
		}
	}

	//- 국가 게임오버
	void gameOver();
	bool isLive() { return _isLive; }
};