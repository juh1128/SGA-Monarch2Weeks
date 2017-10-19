#pragma once
#include "gameObject.h"

class unit;

class userInterface : public gameObject
{
private:
	tileMap*			_map;
	unit*				_pickedUnit;
	vector2D			_rbDownPos;

public:
	userInterface() {}
	virtual ~userInterface() {}

	HRESULT init();
	void release();
	void render();
	void update();

	//ī�޶� �̵�
	void moveCamera();
	//���� ��ŷ
	void pickUnit();



	//��ŷ�� Ÿ�� ���� ������Ʈ ���� ǥ��
	void renderPickInfo();
};