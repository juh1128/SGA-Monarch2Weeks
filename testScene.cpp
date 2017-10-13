#include "stdafx.h"
#include "testScene.h"

HRESULT testScene::init()
{
	sceneBase::init();

	//타일맵 로드
	WORLD->loadTiles("resource/mapData/test.map");

	//카메라 설정
	vector2D mapSize = WORLD->getMapSize();
	CAMERA->setMapSize(mapSize.x, mapSize.y);

	_unitOne = new unit;
	_unitOne->init();

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

	_unitOne->update();
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

	_unitOne->render();
}
