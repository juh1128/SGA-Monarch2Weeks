#include "stdafx.h"
#include "cameraManager.h"

HRESULT cameraManager::init()
{
	_pos.x = 0;
	_pos.y = 0;
	_mapSize.x = WINSIZEX;
	_mapSize.y = WINSIZEY;
	
	_zoom = 1.0f;
	_target = NULL;
	setupRenderRC();

	return S_OK;
}

RECT cameraManager::getRenderRect()
{
	return _renderRC;
}


void cameraManager::release()
{
}

void cameraManager::update()
{
	if (_target)
	{
		RECT rc = RectMakeCenter(_target->_pos.x, _target->_pos.y, WINSIZEX, WINSIZEY);
		_pos.x = rc.left;
		_pos.y = rc.top;
		setupRenderRC();
	}

	////렌더링 위치가 맵 사이즈를 벗어나면 조정해준다.
	//_pos.fixedPosX(-_mapSize.x*_zoom, _mapSize.x*_zoom);
	//_pos.fixedPosY(-_mapSize.y*_zoom, _mapSize.y*_zoom);
}

void cameraManager::setTarget(gameObject * obj)
{
	_target = obj;
}



void cameraManager::setupRenderRC()
{
	//카메라 위치를 LeftTop으로 렉트를 만든다.
	_renderRC = RectMake((int)_pos.x, (int)_pos.y, WINSIZEX, WINSIZEY);
}

RECT cameraManager::getRelativeRect(RECT rc)
{
	MoveRect(&rc, -_pos.x, -_pos.y);
	return std::move(rc);
}
POINT cameraManager::getRelativePoint(POINT pt)
{
	pt.x -= _pos.x;
	pt.y -= _pos.y;
	return std::move(pt);
}
vector2D cameraManager::getRelativeVector2D(vector2D v)
{
	v.x -= _pos.x;
	v.y -= _pos.y;
	return std::move(v);
}

vector2D cameraManager::getZoomedPos(vector2D v)
{
	return v*_zoom;
}

POINT cameraManager::getMousePos()
{
	POINT pt = { _ptMouse.x + _pos.x, _ptMouse.y + _pos.y };
	return std::move(pt);
}
