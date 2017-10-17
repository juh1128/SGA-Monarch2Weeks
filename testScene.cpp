#include "stdafx.h"
#include "testScene.h"
#include "objectFactory.h"

HRESULT testScene::init()
{
	sceneBase::init();

	//타일맵 로드
	WORLD->loadTiles("resource/mapData/stage1.map");

	//카메라 설정
	vector2D mapSize = WORLD->getMapSize();
	CAMERA->setMapSize(mapSize.x, mapSize.y);

	//배경음악 재생
	SOUNDMANAGER->play("bgm", 0.5f);

	//테스트 마을 하나 생성
	objectFactory factory;
	WORLD->addObject(factory.createObject(4, 8, "redTown"));

	return S_OK;
}

void testScene::release()
{
	sceneBase::release();
}

void testScene::resume()
{
}

void testScene::update()
{
	sceneBase::update();

	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
	{
		unit* _unit = new unit;
		_unit->init(vector2D(4,8), 0,CountryColor::GREEN);
		WORLD->addUnit(_unit, CountryColor::GREEN);
	}
	

	//if (KEYMANAGER->isOnceKeyDown(VK_F1))
	//{
	//	this->sendMessage("disableWorld");
	//}
	//if (KEYMANAGER->isOnceKeyDown(VK_F2))
	//{
	//	this->sendMessage("enableWorld");
	//}

}

void testScene::render()
{
	sceneBase::render();
}
