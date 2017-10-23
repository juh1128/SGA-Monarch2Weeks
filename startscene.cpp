#include "stdafx.h"
#include "startscene.h"

startscene::startscene(){}
startscene::~startscene(){}

HRESULT startscene::init()
{
	sceneBase::init();
	SOUNDMANAGER->play("titleBgm", 0.5f);

	word = true; 
	click = false; 

	IMAGEMANAGER->addImage("titleBack", L"resource/startscene/title.png");
	
	rc_1 = RectMakeCenter(60+80, 75+500, 150, 20);
	rc_2 = RectMakeCenter(60+80, 105+500, 150, 20);

	_frame = 0;
	return S_OK;
}

void startscene::release()
{
	sceneBase::release();

}

void startscene::resume()
{
	SOUNDMANAGER->stop("bgm");
	SOUNDMANAGER->play("titleBgm", 0.5f);
}

void startscene::update()
{
	sceneBase::update();
	
	//if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON)) word = true; 

	//if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON)) click = true;
	//else (click = false);
	
	if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
	{
		SCENEMANAGER->pushScene(new testScene);
		SOUNDMANAGER->stop("titleBgm");
	}
	

	if(_frame < 22)
		_frame = ++_frame;

}

void startscene::render()
{
	sceneBase::render();

	IMAGEMANAGER->findImage("titleBack")->render(0, 0, Pivot::LEFT_TOP, false);
	IMAGEMANAGER->findImage("title")->frameRender(WINSIZEX/2, 250, 0, _frame, Pivot::CENTER, false);

	IMAGEMANAGER->drawTextField(0, 673, L"게임을 시작하려면 엔터를 누르세요.", 30, 1024, 80);

	//if (word == true)
	//{

	//	char buf[128] = "처음부터";
	//	IMAGEMANAGER->drawText(rc_1.left, rc_1.top, UTIL::string_to_wstring(buf), 20, DefaultBrush::white);
	//	char buf1[128] = "종료";
	//	IMAGEMANAGER->drawText(rc_2.left, rc_2.top, UTIL::string_to_wstring(buf1), 20, DefaultBrush::white);

	//	//

	//	if (PtInRect(&rc_1, _ptMouse))
	//	{
	//		IMAGEMANAGER->drawText(rc_1.left, rc_1.top, UTIL::string_to_wstring(buf), 20, DefaultBrush::white);
	//	}
	//	if (PtInRect(&rc_2, _ptMouse))
	//	{
	//		IMAGEMANAGER->drawText(rc_2.left, rc_2.top, UTIL::string_to_wstring(buf1), 20, DefaultBrush::white);
	//	}

	//	//	
	//
	//	if (PtInRect(&rc_2, _ptMouse) && click == true )
	//	{
	//		exit(0);
	//	}
	//}
	//
	
	
	
	
	

	

}


