#include "stdafx.h"
#include "userInterface.h"





HRESULT interfaceBack::init()
{
	gameObject::init("인터페이스배경", "interfaceBack", vector2D(0, 0), Pivot::LEFT_TOP);
	return S_OK;
}

void interfaceBack::release()
{
	gameObject::release();
}

void interfaceBack::update()
{
	gameObject::update();
}

void interfaceBack::render()
{
	_image->render(_pos.x, _pos.y, Pivot::LEFT_TOP, false);
}



//세금 프로그레스바
HRESULT taxProgress::init(gameObject* parent, float taxRate)
{
	_parent = parent;
	_relativePos = vector2D(63, 8);
	_taxRate = taxRate;

	gameObject::init("세금게이지", "taxProgress", _parent->_pos + _relativePos, Pivot::LEFT_TOP);

	return S_OK;
}

void taxProgress::release()
{
	gameObject::release();
}

void taxProgress::update()
{
	gameObject::update();

	//위치 싱크
	_pos = _parent->_pos + _relativePos;

	//피킹
	if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
	{
		RECT rc = RectMake(_pos.x, _pos.y, _size.x, _size.y);
		if (PtInRect(&rc, _ptMouse))
		{
			vector2D relativeMousePos = vector2D(_ptMouse) - _pos;
			_taxRate = (relativeMousePos.x / _size.x) * 0.3f;
		}
	}
}

void taxProgress::render()
{
	_scale.x = 1.0f;
	_image->frameRender(_pos.x, _pos.y, 0, 0, Pivot::LEFT_TOP, false); //백

	float width = _size.x;
	_scale.x = _taxRate / 0.3f;

	_image->setScaleOption(_scale);
	_image->frameRender(_pos.x+1, _pos.y, 0, 1, Pivot::LEFT_TOP, false); //프론트

	//텍스트 출력
	char buf[128] = "";
	sprintf(buf, "%.0f%%", _taxRate * 100);
	IMAGEMANAGER->drawText(_pos.x+_size.x*0.4f, _pos.y, UTIL::string_to_wstring(buf), 16, DefaultBrush::white);
}


//세율 오토 버튼
HRESULT autoButton::init(gameObject* parent)
{
	_parent = parent;
	_relativePos = vector2D(7, 9);
	_auto = true;

	gameObject::init("오토버튼", "autoBtn", _parent->_pos + _relativePos, Pivot::LEFT_TOP);
	return S_OK;
}

void autoButton::release()
{
	gameObject::release();
}

void autoButton::update()
{
	gameObject::update();

	//위치 싱크
	_pos = _parent->_pos + _relativePos;

	//버튼 충돌 체크
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		RECT rc = RectMake(_pos.x, _pos.y, _size.x, _size.y);
		if (PtInRect(&rc, _ptMouse))
		{
			_auto = !_auto;
		}
	}
}

void autoButton::render()
{
	_image->frameRender(_pos.x, _pos.y, !_auto, 0, Pivot::LEFT_TOP, false);
}
