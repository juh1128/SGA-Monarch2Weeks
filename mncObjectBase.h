#pragma once
#include "gameObject.h"

class mncObjectBase : public gameObject
{
protected:
	POINT					_index;
	int						_height;
	int						_hp;
	int						_maxHp;
	bool					_walkable;

	CountryColor::Enum		_countryColor;
	int						_cost;

public:
	mncObjectBase() {}
	virtual ~mncObjectBase() {}

	virtual HRESULT init(string name, string key, int xIndex, int yIndex, int hp, bool walkable);
	virtual void release();
	virtual void update();
	virtual void render();
	void frameRender(int xFrame, int yFrame);


	int getHp() { return _hp; }
	void addHp(int add) { _hp += add; }
	void setHp(int set) { _hp = set; }

	void setMaxHp(int set) { _maxHp = set; }
	int getMaxHp() { return _maxHp; }
	void setCost(int set) { _cost = set; }
	int getCost() { return _cost; }

	vector2D getIndex() { return _index; }
	CountryColor::Enum getCountryColor() { return _countryColor; }
	void setCountryColor(CountryColor::Enum color) { _countryColor = color; }
};