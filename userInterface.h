#pragma once
#include "gameObject.h"


class userInterface : public gameObject
{
private:
	tileMap*			_map;
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
	//Ÿ�� ��ŷ
	void pickingTile();

	//��ŷ�� Ÿ�� ���� ������Ʈ ���� ǥ��
	void renderPickInfo();
};