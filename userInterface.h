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

	//�������̽� ��ü��...
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

	//ī�޶� �̵�
	void moveCamera();
	//���� ��ŷ
	void pickUnit();



	//��ŷ�� Ÿ�� ���� ������Ʈ ���� ǥ��
	void renderPickInfo();
	//�� ������ ��Ȳ�� ������
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