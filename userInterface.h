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
	image*				_whatwhereImage;

public:
	userInterface() {}
	virtual ~userInterface() {}

	HRESULT init(CountryColor::Enum playerColor);
	void release();
	void render();
	void update();

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