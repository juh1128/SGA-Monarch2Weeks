#include "stdafx.h"
#include "playGround.h"
#include "testScene.h"
#include "startscene.h"
#include "objectFactory.h"


playGround::playGround()
{
}


playGround::~playGround()
{
}

HRESULT playGround::init(void)
{
	gameNode::init(true);

	//리소스 로드
	// - 유닛
	IMAGEMANAGER->addFrameImage("greenKing", L"resource/unit/greenKing.png", 2, 4, false);
	IMAGEMANAGER->addFrameImage("greenLv1", L"resource/unit/greenLv1.png", 2, 4, false);
	IMAGEMANAGER->addFrameImage("greenLv2", L"resource/unit/greenLv2.png", 2, 4, false);
	IMAGEMANAGER->addFrameImage("greenLv3", L"resource/unit/greenLv3.png", 2, 4, false);
	IMAGEMANAGER->addFrameImage("redKing", L"resource/unit/orangeKing.png", 2, 4, false);
	IMAGEMANAGER->addFrameImage("redLv1", L"resource/unit/orangeLv1.png", 2, 4, false);
	IMAGEMANAGER->addFrameImage("redLv2", L"resource/unit/orangeLv2.png", 2, 4, false);
	IMAGEMANAGER->addFrameImage("redLv3", L"resource/unit/orangeLv3.png", 2, 4, false);
	IMAGEMANAGER->addFrameImage("blueKing", L"resource/unit/blueKing.png", 2, 4, false);
	IMAGEMANAGER->addFrameImage("blueLv1", L"resource/unit/blueLv1.png", 2, 4, false);
	IMAGEMANAGER->addFrameImage("blueLv2", L"resource/unit/blueLv2.png", 2, 4, false);
	IMAGEMANAGER->addFrameImage("blueLv3", L"resource/unit/blueLv3.png", 2, 4, false);
	//오브젝트 팩토리 초기화
	objectFactory::initUnitImage();

	// - 오브젝트
	IMAGEMANAGER->addFrameImage("bridge", L"resource/object/bridge.png", 2, 1);
	IMAGEMANAGER->addFrameImage("castle", L"resource/object/castle.png", 4, 1);
	IMAGEMANAGER->addFrameImage("farmLand", L"resource/object/farmLand.png", 4, 1);
	IMAGEMANAGER->addFrameImage("town", L"resource/object/town.png", 4, 1);
	IMAGEMANAGER->addFrameImage("tree", L"resource/object/tree.png", 3, 1);
	IMAGEMANAGER->addFrameImage("wall", L"resource/object/wall.png", 3, 1);
	IMAGEMANAGER->addImage("rock", L"resource/object/rock.png");
	IMAGEMANAGER->addImage("weed", L"resource/object/weed.png");

	// - 배경음악
	SOUNDMANAGER->addSound("bgm", "resource/bgm/field.mp3", true, true);


	//op 
	IMAGEMANAGER->addFrameImage("OP_1", L"resource/startscene/O_000.png", 1, 50, false);
	IMAGEMANAGER->addFrameImage("title", L"resource/startscene/O_001.png", 1, 50, false);



	//초기 시작 씬 생성
	SCENEMANAGER->pushScene(new startscene);
	

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


//다이렉트2D 메인 렌더링~!
void playGround::render(void)
{
	auto renderTarget = RT;
	//윈도우가 가려져있는지 체크
	D2D1_WINDOW_STATE eWindowState = renderTarget->CheckWindowState();
	if (!(eWindowState & D2D1_WINDOW_STATE_OCCLUDED))
	{
		renderTarget->BeginDraw();
		renderTarget->Clear();

		renderTarget->SetTransform(D2D1::IdentityMatrix());
		SCENEMANAGER->render();

		HRESULT hr = renderTarget->EndDraw();
		//디바이스 로스트 체크
		if (hr == D2DERR_RECREATE_TARGET)
		{
			IMAGEMANAGER->reloadALL();
		}
	}
}
