#include "stdafx.h"
#include "unit.h"


unit::unit()
{
}


unit::~unit()
{
}

HRESULT unit::init()
{
	gameObject::init("РЏДж", "unit");

	return S_OK;
}

void unit::release()
{
}

void unit::update()
{
}

void unit::render()
{
	_scale = vector2D(CAMERA->getZoom(),CAMERA->getZoom());
	gameObject::frameRender(0, 0);
}
