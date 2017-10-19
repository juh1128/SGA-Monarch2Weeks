#pragma once
#include "gameObject.h"

class unit;
class taxProgress;
class autoButton;
class interfaceBack;

class userInterface : public gameObject
{
private:
	country*			_playerCountry;

	tileMap*			_map;
	unit*				_pickedUnit;
	vector2D			_rbDownPos;

	//인터페이스 객체들...
	interfaceBack*		_back;
	autoButton*			_autoBtn;
	taxProgress*		_taxProgress;
	image*				_otherCountryInfo;
	image*				_countryColorSprite;

public:
	userInterface() {}
	virtual ~userInterface() {}

	HRESULT init(CountryColor::Enum playerColor);
	void release();
	void render();
	void update();

	//카메라 이동
	void moveCamera();
	//유닛 피킹
	void pickUnit();



	//피킹된 타일 위의 오브젝트 정보 표시
	void renderPickInfo();
	//각 국가별 현황판 렌더링
	void renderCountryInfo();
};


class interfaceBack : public gameObject
{
private:
	
public:
	interfaceBack() {}
	virtual ~interfaceBack() {}

	HRESULT init();
	void release();
	void update();
	void render();
};

class taxProgress : public gameObject
{
private:
	gameObject* _parent;
	vector2D _relativePos;

	float _taxRate;
public:
	taxProgress() {}
	virtual ~taxProgress() {}

	HRESULT init(gameObject* parent, float taxRate);
	void release();
	void update();
	void render();

	float getTaxRate() { return _taxRate; }
};

class autoButton : public gameObject
{
private:
	gameObject* _parent;
	vector2D _relativePos;

	bool	_auto;

public:
	autoButton() {}
	virtual ~autoButton() {}

	HRESULT init(gameObject* parent);
	void release();
	void update();
	void render();

	bool getAutoState() { return _auto; }
};