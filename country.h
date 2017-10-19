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
	int							_gold;   //��
	float						_taxRate; //����

	vector<unit*>				_unitList; //���ָ���Ʈ
	int							_countryPower; //������

public:
	country() {}
	~country() {}

	void init(CountryColor::Enum color);
	void release();
	void update();
	void render();

	void addUnit(unit* newUnit);
	vector<unit*>* getUnitList() { return &_unitList; }

	//- �� ����
	int getGold() { return _gold; }
	void addGold(int add) { _gold += add; }
	void setGold(int set) { _gold = set; }
	float getTaxRate() { return _taxRate; }
	void setTaxRate(float set) { _taxRate = set; }

	//- ��Ÿ
	int getCountryPower() { return _countryPower; }
	CountryColor::Enum getColor() { return _countryColor; }
};