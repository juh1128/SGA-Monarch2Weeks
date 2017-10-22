#include "stdafx.h"
#include "selectScene.h"

HRESULT selectScene::init()
{
	sceneBase::init();

	//ī�޶� ����
	CAMERA->setMapSize(WINSIZEX, WINSIZEY);

	//������� ���
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
