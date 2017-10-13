#include "stdafx.h"
#include "playGround.h"
#include "testScene.h"

playGround::playGround()
{
}


playGround::~playGround()
{
}

HRESULT playGround::init(void)
{
	gameNode::init(true);

	//�ʱ� ���� �� ����
	SCENEMANAGER->pushScene(new testScene);
	IMAGEMANAGER->addFrameImage("unit", L"resource/unit/greenLv1.png",8,1,false);

	return S_OK;
}

void playGround::release(void)
{
	gameNode::release();
}

void playGround::update(void)
{
	gameNode::update();

	if (KEYMANAGER->isOnceKeyDown(VK_F1))
	{
		_isDebugMode = !_isDebugMode;
	}
	
	SCENEMANAGER->update();
}


//���̷�Ʈ2D ���� ������~!
void playGround::render(void)
{
	auto renderTarget = RT;
	//�����찡 �������ִ��� üũ
	D2D1_WINDOW_STATE eWindowState = renderTarget->CheckWindowState();
	if (!(eWindowState & D2D1_WINDOW_STATE_OCCLUDED))
	{
		renderTarget->BeginDraw();
		renderTarget->Clear();

		renderTarget->SetTransform(D2D1::IdentityMatrix());
		SCENEMANAGER->render();

		HRESULT hr = renderTarget->EndDraw();
		//����̽� �ν�Ʈ üũ
		if (hr == D2DERR_RECREATE_TARGET)
		{
			IMAGEMANAGER->reloadALL();
		}
	}
}
