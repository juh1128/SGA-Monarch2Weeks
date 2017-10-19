#include "stdafx.h"
#include "testScene.h"
#include "objectFactory.h"
#include "userInterface.h"

HRESULT testScene::init()
{
	sceneBase::init();

	//타일맵 로드
	WORLD->loadTiles("resource/mapData/stage1.map");

	//카메라 설정
	vector2D mapSize = WORLD->getMapSize();
	CAMERA->setMapSize(mapSize.x, mapSize.y);

	//UI
	_ui = new userInterface;
	_ui->init(CountryColor::BLUE);

	//배경음악 재생
	SOUNDMANAGER->play("bgm", 0.5f);

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

	//인터페이스
	_ui->update();

	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
	{
		unit* _unit = new unit;
		_unit->init(vector2D(4,8), 0,CountryColor::GREEN);
		WORLD->addUnit(_unit, CountryColor::GREEN);
	}
	if (KEYMANAGER->isOnceKeyDown('R'))
	{
		unit* _unit = new unit;
		_unit->init(vector2D(4, 8), 0, CountryColor::RED);
		_unit->setHp(400);
		WORLD->addUnit(_unit, CountryColor::RED);
	}
	if (KEYMANAGER->isOnceKeyDown('B'))
	{
		unit* _unit = new unit;
		_unit->init(vector2D(4, 8), 0, CountryColor::BLUE);
		_unit->setHp(400);
		WORLD->addUnit(_unit, CountryColor::BLUE);
	}
	if (KEYMANAGER->isOnceKeyDown('A'))
	{
		unit* _unit = new unit;
		_unit->init(vector2D(4, 8), 0, CountryColor::BLUE);
		_unit->setHp(400);
		WORLD->addUnit(_unit, CountryColor::BLUE);
		unit* _unit2 = new unit;
		_unit2->init(vector2D(5, 8), 0, CountryColor::RED);
		_unit2->setHp(400);
		WORLD->addUnit(_unit2, CountryColor::RED);
		unit* _unit3 = new unit;
		_unit3->init(vector2D(6, 8), 0, CountryColor::BLUE);
		_unit3->setHp(400);
		WORLD->addUnit(_unit3, CountryColor::BLUE);

	}




}

void testScene::render()
{
	sceneBase::render();

	//인터페이스 렌더링
	_ui->render();
}
