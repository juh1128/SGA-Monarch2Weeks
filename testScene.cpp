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
	if (KEYMANAGER->isOnceKeyDown('B'))
	{
		unit* _unit = new unit;
		_unit->init(vector2D(4, 8), 0, CountryColor::RED);
		_unit->setHp(400);
		WORLD->addUnit(_unit, CountryColor::RED);
	}
	
	//마우스 피킹 + 유닛피킹 시 WORLD 스탑
	_world->getMap()->setMousePickTile();
	terrainTile* tile = _world->getMap()->getPickedTile();
	if (tile)
	{
		if (tile->getUnitOnTile().size() > 0)
		{
			this->enableWorld(false);
		}
		else
		{
			this->enableWorld(true);
		}
	}

	//카메라 이동 구현
	if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
	{
		_rbDownPos = _ptMouse;
	}
	if (KEYMANAGER->isStayKeyDown(VK_RBUTTON))
	{
		CAMERA->setPos(CAMERA->getPos() + _rbDownPos - vector2D(_ptMouse));
		_rbDownPos = _ptMouse;
	}


}

void testScene::render()
{
	sceneBase::render();
}
