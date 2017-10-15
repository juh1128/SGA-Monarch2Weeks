#pragma once
#include "singletonBase.h"

class gameObject;

#define MAX_ZOOM 5.0f
#define MIN_ZOOM 0.5f

class cameraManager : public singletonBase<cameraManager>
{
private:
	vector2D	_pos;
	vector2D	_mapSize;
	float		_zoom;

	RECT		_renderRC;
	gameObject* _target;

public:
	HRESULT init();
	void release();
	void update();

	void setTarget(gameObject* obj);

	void setMapSize(int width, int height)
	{
		_mapSize.x = (float)width; _mapSize.y = (float)height;
	}
	vector2D getMapSize() { return _mapSize; }

	RECT getRenderRect();
	void setPos(vector2D pos) { _pos = pos; setupRenderRC(); }
	vector2D getPos() { return _pos; }

	RECT getRelativeRect(RECT rc);
	POINT getRelativePoint(POINT pt);
	vector2D getRelativeVector2D(vector2D v);
	POINT getMousePos();

	void setZoom(float zoom)
	{
		_zoom = zoom;
		if (_zoom < MIN_ZOOM) _zoom = MIN_ZOOM;
		if (_zoom > MAX_ZOOM) _zoom = MAX_ZOOM;
	}
	float getZoom() { return _zoom; }

	cameraManager() {}
	~cameraManager() {}

private:
	void setupRenderRC();
};

