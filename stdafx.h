#pragma once

// Windows 헤더 파일:
#include <windows.h>
#include <Commdlg.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <iostream>
#include <memory.h>
#include <tchar.h>

using namespace std;

#include "commonMacroFunction.h"
#include "randomFunction.h"
#include "vector2D.h"
#include "utils.h"
#include "json.h"
#include "animation.h"

#include "imageManager.h"
#include "keyManager.h"
#include "soundManager.h"
#include "timeManager.h"
#include "cameraManager.h"
//#include "effectManager.h"
#include "sceneManager.h"
//#include "keyAniManager.h"

using namespace UTIL;

//콘솔창 띄우기
#pragma comment(linker , "/entry:WinMainCRTStartup /subsystem:console")

//=======================================
// ## 전역 인클루드 ##
//=======================================
#include "tagMessage.h"
#include "animation.h"
#include "gameObject.h"
#include "world.h"
#include "sceneBase.h"

#include "pathFinder.h"

//========================================
// ## 디파인 문 ## 17.07.03 ##
//========================================

#define WINNAME (LPTSTR)(TEXT("SGA 모나크모나크"))
#define WINSTARTX 80
#define WINSTARTY 0
#define WINSIZEX 1024
#define WINSIZEY 768
#define WINSTYLE WS_CAPTION | WS_SYSMENU

//=============================================================
// ## 17.07.06 ## Singleton Define ##
//=============================================================

#define RND randomFunction::getSingleton()
#define KEYMANAGER keyManager::getSingleton()
#define SOUNDMANAGER soundManager::getSingleton()
#define TIMEMANAGER timeManager::getSingleton()
#define CAMERA cameraManager::getSingleton()
//#define EFFECTMANAGER effectManager::getSingleton()
#define SCENEMANAGER sceneManager::getSingleton()
#define PATHFINDER pathFinder::getSingleton()

#define WORLD SCENEMANAGER->getNowScene()->getWorld()

//=============================================================
// ## 17.07.06 ## Macro ##
//=============================================================

#define SAFE_DELETE(p) {if(p) {delete(p); (p) = NULL;}}
#define SAFE_RELEASE(p) {if(p) {(p)->release(); (p)=NULL;}}
#define SAFE_DELETE_ARRAY(p) {if(p) { delete[](p); (p) = NULL;}}

//다이렉트 2D 클래스들 해제 시 사용
template <typename T>
inline void safeRelease(T *&p)
{
	if (NULL != p)
	{
		p->Release();
		p = NULL;
	}
}

//=============================================================
// ## 17.07.06 ## 전역변수 ##
//=============================================================

extern HINSTANCE _hInstance;
extern HWND _hWnd;
extern POINT _ptMouse;
extern bool _isDebugMode;