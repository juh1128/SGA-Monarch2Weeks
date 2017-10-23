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

	IMAGEMANAGER->addImage("titleBack", L"resource/tiles/background/background2.png");
	
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
}

void startscene::update()
{
	sceneBase::update();
	
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON)) word = true; 

	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON)) click = true;
	else (click = false);
	
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON) && click == true && PtInRect(&rc_1, _ptMouse))
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

	IMAGEMANAGER->findImage("titleBack")->setSizeOption(vector2D(WINSIZEX, WINSIZEY));
	IMAGEMANAGER->findImage("titleBack")->render(0, 0, Pivot::LEFT_TOP, false);
	IMAGEMANAGER->findImage("title")->frameRender(WINSIZEX/2, 250, 0, _frame, Pivot::CENTER, false);

	if (word == true)
	{

		char buf[128] = "처음부터";
		IMAGEMANAGER->drawText(rc_1.left, rc_1.top, UTIL::string_to_wstring(buf), 20, DefaultBrush::black);
		char buf1[128] = "종료";
		IMAGEMANAGER->drawText(rc_2.left, rc_2.top, UTIL::string_to_wstring(buf1), 20, DefaultBrush::black);

		//

		if (PtInRect(&rc_1, _ptMouse))
		{
			IMAGEMANAGER->drawText(rc_1.left, rc_1.top, UTIL::string_to_wstring(buf), 20, DefaultBrush::white);
		}
		if (PtInRect(&rc_2, _ptMouse))
		{
			IMAGEMANAGER->drawText(rc_2.left, rc_2.top, UTIL::string_to_wstring(buf1), 20, DefaultBrush::white);
		}

		//	
	
		if (PtInRect(&rc_2, _ptMouse) && click == true )
		{
			exit(0);
		}
	}
	//
	
	
	
	
	

	

}


