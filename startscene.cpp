#include "stdafx.h"
#include "startscene.h"

startscene::startscene(){}
startscene::~startscene(){}

HRESULT startscene::init()
{
	sceneBase::init();
	// SOUNDMANAGER->play(*,0.5f)
	word = false; 
	click = false; 
	
	
	rc_1 = RectMakeCenter(60, 75, 150, 20);
	rc_2 = RectMakeCenter(60, 105, 150, 20);
	rc_3 = RectMakeCenter(60, 135, 100, 20); 

	

	

	
	
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
		SCENEMANAGER->pushScene(new testScene);
	

	


}

void startscene::render()
{
	sceneBase::render();

	if (word == true)
	{

		char buf[128] = "처음부터";
		IMAGEMANAGER->drawText(50, 70, UTIL::string_to_wstring(buf), 20, DefaultBrush::gray);
		char buf1[128] = "이어하기";
		IMAGEMANAGER->drawText(50, 100, UTIL::string_to_wstring(buf1), 20, DefaultBrush::gray);
		char buf2[128] = "종료";
		IMAGEMANAGER->drawText(50, 130, UTIL::string_to_wstring(buf2), 20, DefaultBrush::gray);

		//

		if (PtInRect(&rc_1, _ptMouse))
		{
			IMAGEMANAGER->drawText(50, 70, UTIL::string_to_wstring(buf), 20, DefaultBrush::white);
		}
		if (PtInRect(&rc_2, _ptMouse))
		{
			IMAGEMANAGER->drawText(50, 100, UTIL::string_to_wstring(buf1), 20, DefaultBrush::white);
		}

		if (PtInRect(&rc_3, _ptMouse))
		{
			IMAGEMANAGER->drawText(50, 130, UTIL::string_to_wstring(buf2), 20, DefaultBrush::white);
		}

		//	


		if (PtInRect(&rc_2, _ptMouse) && word == true)
		{
			char buf3[128] = "??";
			IMAGEMANAGER->drawText(130, 100, UTIL::string_to_wstring(buf3), 20, DefaultBrush::gray);

		}

		if (PtInRect(&rc_3, _ptMouse) && click == true )
		{
			exit(0);
		}
	}
	//
	
	
	

	

	

}


