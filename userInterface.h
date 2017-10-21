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

	//�������̽� ��ü��...
	interfaceBack*		_back;
	startButton*		_startBtn;
	taxProgress*		_taxProgress;
	commandWindow*		_commandWindow;

	//�̹���
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

	//ī�޶� �̵�
	void moveCamera();
	//���� ��ŷ
	void pickUnit();
	//Ŭ�� �� ó��
	void clickedMouse();
	//�巡�� �� ó��
	void dragedMouse();


	//��ŷ�� Ÿ�� ���� ������Ʈ ���� ǥ��
	void renderPickInfo();
	//�� ������ ��Ȳ�� ������
	void renderCountryInfo();
};

namespace commandWindowState
{
	enum Enum
	{
		Where, What, Hide, End
	};
}

//��� â
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

	//�޴�
	int					_chooseIndex;
	terrainTile*		_chooseTile;
	int					_menuWidth;
	int					_menuHeight;

	//�巡�� ������ Ȯ�ο�
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