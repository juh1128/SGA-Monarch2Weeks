#include "stdafx.h"
#include "selectScene.h"

HRESULT selectScene::init()
{
	sceneBase::init();

	//카메라 설정
	CAMERA->setMapSize(WINSIZEX, WINSIZEY);

	//배경음악 재생
	SOUNDMANAGER->play("bgm", 0.5f);

	return S_OK;
}

void selectScene::release()
{
	sceneBase::release();
}

void selectScene::resume()
{
}

void selectScene::update()
{
	sceneBase::update();

	
}

void selectScene::render()
{
	sceneBase::render();
}
