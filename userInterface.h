#pragma once
#include "gameObject.h"

class unit;
class taxProgress;
class startButton;
class interfaceBack;
class commandWindow;

class userInterface : public gameObject
{
private:
	country*			_playerCountry;

	tileMap*			_map;
	unit*				_pickedUnit;
	vector2D			_rbDownPos;

	//인터페이스 객체들...
	interfaceBack*		_back;
	startButton*		_startBtn;
	taxProgress*		_taxProgress;
	commandWindow*		_commandWindow;

	//이미지
	image*				_otherCountryInfo;
	image*				_countryColorSprite;
	image*				_whatwhereImage;

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
	//클릭 시 처리
	void clickedMouse();
	//드래그 시 처리
	void dragedMouse();


	//피킹된 타일 위의 오브젝트 정보 표시
	void renderPickInfo();
	//각 국가별 현황판 렌더링
	void renderCountryInfo();
};

namespace commandWindowState
{
	enum Enum
	{
		Show, Hide, End
	};
}
class commandWindow : public gameObject
{
private:
	commandWindowState::Enum		_state;
	unit*							_target;
	

public:
	commandWindow() {}
	virtual ~commandWindow() {}

	HRESULT init();
	void release();
	void update();
	void render();

	void show(unit* target);
	void hide();
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

class startButton : public gameObject
{
private:
	gameObject* _parent;
	vector2D _relativePos;

public:
	startButton() {}
	virtual ~startButton() {}

	HRESULT init(gameObject* parent);
	void release();
	void update();
	void render();
};