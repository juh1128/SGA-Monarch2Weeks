#include "stdafx.h"
#include "testScene.h"

HRESULT testScene::init()
{
	sceneBase::init();

	//타일맵 로드
	WORLD->loadTiles("resource/mapData/stage1.map");

	//카메라 설정
	vector2D mapSize = WORLD->getMapSize();
	CAMERA->setMapSize(mapSize.x, mapSize.y);

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
		_unit->init(vector2D(4,8), 0);
		WORLD->addUnit(_unit, CountryColor::RED);
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
