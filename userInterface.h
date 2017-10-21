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

public:
	userInterface() {}
	virtual ~userInterface() {}

	HRESULT init(CountryColor::Enum playerColor);
	void release();
	void render();
	void update();

	unit* getPickedUnit() { return _pickedUnit; }
	country* getPlayerCountry() { return _playerCountry; }

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
		Where, What, Hide, End
	};
}

//명령 창
class commandWindow : public gameObject
{
private:
	userInterface*		_parent;

	commandWindowState::Enum		_state;
	vector<unit*>					_targetList;
	vector<string>					_menuList;
	
	image*				_whatwhereImage;
	terrainTile*		_destTile;
	unit*				_destUnit;
	vector2D			_renderPos;

	//메뉴
	int					_chooseIndex;
	terrainTile*		_chooseTile;
	int					_menuWidth;
	int					_menuHeight;

	//드래그 중인지 확인용
	vector2D			_clickedPos;

public:
	commandWindow() {}
	virtual ~commandWindow() {}

	HRESULT init(userInterface* parent);
	void release();
	void update();
	void render();

	void show(unit* target);
	void show(vector<unit*> targetList);
	void hide();

	bool setMenuList();
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