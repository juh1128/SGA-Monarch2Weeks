#include "stdafx.h"
#include "userInterface.h"





HRESULT interfaceBack::init()
{
	gameObject::init("�������̽����", "interfaceBack", vector2D(0, 0), Pivot::LEFT_TOP);
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



//���� ���α׷�����
HRESULT taxProgress::init(gameObject* parent, float taxRate)
{
	_parent = parent;
	_relativePos = vector2D(63, 8);
	_taxRate = taxRate;

	gameObject::init("���ݰ�����", "taxProgress", _parent->_pos + _relativePos, Pivot::LEFT_TOP);

	return S_OK;
}

void taxProgress::release()
{
	gameObject::release();
}

void taxProgress::update()
{
	gameObject::update();

	//��ġ ��ũ
	_pos = _parent->_pos + _relativePos;

	//��ŷ
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
	_image->frameRender(_pos.x, _pos.y, 0, 0, Pivot::LEFT_TOP, false); //��

	float width = _size.x;
	_scale.x = _taxRate / 0.3f;

	_image->setScaleOption(_scale);
	_image->frameRender(_pos.x+1, _pos.y, 0, 1, Pivot::LEFT_TOP, false); //����Ʈ

	//�ؽ�Ʈ ���
	char buf[128] = "";
	sprintf(buf, "%.0f%%", _taxRate * 100);
	IMAGEMANAGER->drawText(_pos.x+_size.x*0.4f, _pos.y, UTIL::string_to_wstring(buf), 16, DefaultBrush::white);
}


//���� ��ư
HRESULT startButton::init(gameObject* parent)
{
	_parent = parent;
	_relativePos = vector2D(7, 9);
	SCENEMANAGER->getNowScene()->sendMessage("disableWorld");

	gameObject::init("���۹�ư", "startBtn", _parent->_pos + _relativePos, Pivot::LEFT_TOP);
	return S_OK;
}

void startButton::release()
{
	gameObject::release();
}

void startButton::update()
{
	gameObject::update();

	//��ġ ��ũ
	_pos = _parent->_pos + _relativePos;

	//��ư �浹 üũ
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		RECT rc = RectMake(_pos.x, _pos.y, _size.x, _size.y);
		if (PtInRect(&rc, _ptMouse))
		{
			if (SCENEMANAGER->getNowScene()->isStopedWorld())
			{
				SCENEMANAGER->getNowScene()->sendMessage("enableWorld");
			}
			else
			{
				SCENEMANAGER->getNowScene()->sendMessage("disableWorld");
			}
		}
	}
}

void startButton::render()
{
	bool isStop = SCENEMANAGER->getNowScene()->isStopedWorld();
	_image->frameRender(_pos.x, _pos.y, !isStop, 0, Pivot::LEFT_TOP, false);
}



//==============================================================
// ��� �������̽�

void commandWindow::show(unit* target)
{
	_state = commandWindowState::Show;
	_target = target;
	SCENEMANAGER->getNowScene()->sendMessage("disableWorld");
}
void commandWindow::hide()
{
	_state = commandWindowState::Hide;
	_target = NULL;
	SCENEMANAGER->getNowScene()->sendMessage("enableWorld");
}

HRESULT commandWindow::init()
{
	hide();
	return S_OK;
}

void commandWindow::release()
{
	gameObject::release();
}

void commandWindow::update()
{
	gameObject::update();

	//��� â �浹üũ
	if (_state == commandWindowState::Show)
	{

	}
}

void commandWindow::render()
{
	//��� â ������
	if (_state == commandWindowState::Show)
	{

	}
}
