#include "stdafx.h"
#include "testScene.h"
#include "objectFactory.h"
#include "userInterface.h"

HRESULT testScene::init()
{
	sceneBase::init();

	//Ÿ�ϸ� �ε�
	WORLD->loadTiles("resource/mapData/stage1.map");

	//ī�޶� ����
	vector2D mapSize = WORLD->getMapSize();
	CAMERA->setMapSize(mapSize.x, mapSize.y);

	//UI
	_ui = new userInterface;
	_ui->init();

	//������� ���
	SOUNDMANAGER->play("bgm", 0.5f);

	//�׽�Ʈ ���� �ϳ� ����
	objectFactory factory;
	WORLD->addObject(factory.createObject(4, 8, "redTown"));

	return S_OK;
}

void testScene::release()
{
	_ui->release();
	delete _ui;

	sceneBase::release();
}

void testScene::resume()
{
}

void testScene::update()
{
	sceneBase::update();

	//�������̽�
	_ui->update();

	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
	{
		unit* _unit = new unit;
		_unit->init(vector2D(4,8), 0,CountryColor::GREEN);
		WORLD->addUnit(_unit, CountryColor::GREEN);
	}
	if (KEYMANAGER->isOnceKeyDown('B'))
	{
		unit* _unit = new unit;
		_unit->init(vector2D(4, 8), 0, CountryColor::RED);
		_unit->setHp(400);
		WORLD->addUnit(_unit, CountryColor::RED);
	}
	



}

void testScene::render()
{
	sceneBase::render();

	//�������̽� ������
	_ui->render();
}
