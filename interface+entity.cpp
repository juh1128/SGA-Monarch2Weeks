#include "stdafx.h"
#include "userInterface.h"
#include "unit.h"
#include "mncObjectBase.h"



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


//시작 버튼
HRESULT startButton::init(gameObject* parent)
{
	_parent = parent;
	_relativePos = vector2D(7, 9);
	SCENEMANAGER->getNowScene()->sendMessage("disableWorld");

	gameObject::init("시작버튼", "startBtn", _parent->_pos + _relativePos, Pivot::LEFT_TOP);
	return S_OK;
}

void startButton::release()
{
	gameObject::release();
}

void startButton::update()
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
// 명령 인터페이스
//클릭 시
void commandWindow::show(unit* target)
{
	if (_state == commandWindowState::Hide)
	{
		_state = commandWindowState::Where;
		_targetList.clear();
		_targetList.push_back(target);
		SCENEMANAGER->getNowScene()->sendMessage("disableWorld");
	}
}
//드래그 시
void commandWindow::show(vector<unit*> targetList)
{
	if (_state == commandWindowState::Hide)
	{
		_state = commandWindowState::Where;
		_targetList.clear();
		for (size_t i = 0; i < targetList.size(); ++i)
		{
			_targetList.push_back(targetList[i]);
		}	
		SCENEMANAGER->getNowScene()->sendMessage("disableWorld");
	}
}

void commandWindow::hide()
{
	_state = commandWindowState::Hide;
	SCENEMANAGER->getNowScene()->sendMessage("enableWorld");
}

HRESULT commandWindow::init()
{
	_whatwhereImage = IMAGEMANAGER->addFrameImage("whatwhere", L"resource/interface/whatwhere.png", 2, 1);
	_destTile = NULL;
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

	switch (_state)
	{
		//어디?
		case commandWindowState::Where:
		{
			//타일 클릭 시 What으로 넘어감.
			if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
			{
				_destTile = WORLD->getMap()->getPickedTile();
				if (_destTile)
				{
					_state = commandWindowState::What;
				}			
			}

			//어디? 일 때 어딜 찍었는지에 따라 명령이 열림.
			if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
			{
				hide();
			}
		}
		break;
		//뭐?
		case commandWindowState::What:
		{
			if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
			{
				_state = commandWindowState::Where;
			}
		}
		break;
	}

}

void commandWindow::render()
{
	float zoom = CAMERA->getZoom();
	unit* target = NULL;
	vector2D renderPos;

	switch (_state)
	{
		//어디?
		case commandWindowState::Where:
		{
			//말풍선 렌더링
			for (size_t i = 0; i < _targetList.size(); ++i)
			{
				target = _targetList[i];
				renderPos = target->_pos * zoom;
				float yOffset = target->getSize().y*zoom*0.5f;
				_whatwhereImage->setScaleOption(vector2D(zoom, zoom));
				_whatwhereImage->frameRender(renderPos.x, renderPos.y - yOffset, 1, 0, Pivot::BOTTOM);
			}
		}
		break;
		//뭐?
		case commandWindowState::What:
		{
			//말풍선 렌더링
			for (size_t i = 0; i < _targetList.size(); ++i)
			{
				target = _targetList[i];
				renderPos = target->_pos * zoom;
				float yOffset = target->getSize().y*zoom*0.5f;
				_whatwhereImage->setScaleOption(vector2D(zoom, zoom));
				_whatwhereImage->frameRender(renderPos.x, renderPos.y - yOffset, 0, 0, Pivot::BOTTOM);
			}
		}
		break;
	}
}
