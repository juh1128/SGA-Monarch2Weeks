#include "stdafx.h"
#include "gameNode.h"


gameNode::gameNode()
{
}


gameNode::~gameNode()
{
	
}

HRESULT gameNode::init(void)
{
	_managerInit = false;
		
	return S_OK;
}

HRESULT gameNode::init(bool managerInit)
{
	_managerInit = managerInit;

	if (_managerInit)
	{
		KEYMANAGER->init();
		IMAGEMANAGER->init();
		SOUNDMANAGER->init();
		TIMEMANAGER->init();
		CAMERA->init();
		//EFFECTMANAGER->init();
	}

	return S_OK;
}

//메모리 해제
void gameNode::release(void)
{
	if (_managerInit)
	{
		KEYMANAGER->releaseSingleton();
		IMAGEMANAGER->releaseSingleton();
		SOUNDMANAGER->releaseSingleton();
		TIMEMANAGER->releaseSingleton();
		CAMERA->releaseSingleton();
		//EFFECTMANAGER->releaseSingleton();
	}
}

//업데이트
void gameNode::update(void)
{

}

//그리기
void gameNode::render(void)
{

}

LRESULT gameNode::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
		case WM_MOUSEMOVE:
			_ptMouse.x = static_cast<float>LOWORD(lParam);
			_ptMouse.y = static_cast<float>HIWORD(lParam);
		break;

		//사이즈 변경 시 체크
		case WM_SIZE:
		{
			UINT width = LOWORD(lParam);
			UINT height = HIWORD(lParam);
			IMAGEMANAGER->onResize(width, height);
		}
		break;

		case WM_LBUTTONDOWN:
			KEYMANAGER->setKeyState(VK_LBUTTON, true);
			break;
		case WM_LBUTTONUP:
			KEYMANAGER->setKeyState(VK_LBUTTON, false);
			break;
		case WM_RBUTTONDOWN:
			KEYMANAGER->setKeyState(VK_RBUTTON, true);
			break;
		case WM_RBUTTONUP:
			KEYMANAGER->setKeyState(VK_RBUTTON, false);
			break;
		case WM_KEYDOWN:
			KEYMANAGER->setKeyState(wParam, true);
			break;
		case WM_KEYUP:
			KEYMANAGER->setKeyState(wParam, false);
			break;

		//휠에 따라서 줌인 줌아웃
		case WM_MOUSEWHEEL:
		{
			float zoom = CAMERA->getZoom();
			float zoomUnit = 0.1f;
			((short)HIWORD(wParam) < 0) ? CAMERA->setZoom(zoom - zoomUnit) : CAMERA->setZoom(zoom + zoomUnit);
		}
		break;

		case WM_DESTROY:
			PostQuitMessage(0);
		break;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
