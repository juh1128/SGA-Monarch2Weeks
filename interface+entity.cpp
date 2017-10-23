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
		_targetList.clear();
		for (size_t i = 0; i < targetList.size(); ++i)
		{
			_targetList.push_back(targetList[i]);
		}	
		SCENEMANAGER->getNowScene()->sendMessage("disableWorld");
		_state = commandWindowState::Where;
	}
}

void commandWindow::hide()
{
	_state = commandWindowState::Hide;
	SCENEMANAGER->getNowScene()->sendMessage("enableWorld");
}

HRESULT commandWindow::init(userInterface* parent)
{
	_parent = parent;
	_whatwhereImage = IMAGEMANAGER->addFrameImage("whatwhere", L"resource/interface/whatwhere.png", 2, 1);
	_destTile = NULL;

	_chooseIndex = -1;
	_menuWidth = 120;
	_menuHeight = 20;

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
				_destUnit = _parent->getPickedUnit();
				if (_destUnit)
				{
					if (_destUnit->isLive())
					{
						vector2D unitIndex = _destUnit->getIndex();
						_chooseObject = _destUnit;

						//뭘 눌렀느냐에 따라 메뉴를 셋팅한다.
						if(setMenuList()) _state = commandWindowState::What;					
					}
				}
				else if (_destTile)
				{
					_chooseObject = _destTile;
					if (setMenuList()) _state = commandWindowState::What;
				}	
				_renderPos = _ptMouse;
			}

			if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
			{
				_clickedPos = _ptMouse;
			}
			if (KEYMANAGER->isOnceKeyUp(VK_RBUTTON))
			{
				if ((_clickedPos - vector2D(_ptMouse)).getLength() < 10)
				{
					hide();
				}
			}
		}
		break;
		//뭐?
		case commandWindowState::What:
		{
			//메뉴 충돌체크
			_chooseIndex = -1;
			for (size_t i = 0; i < _menuList.size(); ++i)
			{
				RECT rc = RectMake(_renderPos.x, _renderPos.y + i * _menuHeight, _menuWidth, _menuHeight);
				if (PtInRect(&rc, _ptMouse))
				{
					_chooseIndex = i;
					break;
				}
			}	

			//메뉴 클릭 시
			if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
			{
				if (_chooseIndex >= 0)
				{
					for (size_t i = 0; i < _targetList.size(); ++i)
					{
						if (_targetList[i]->isLive())
						{
							vector<gameObject*> vTarget;
							vTarget.push_back(_chooseObject);
							_targetList[i]->sendMessage(_menuList[_chooseIndex], 0.0f, 0, 0.0f, POINT(), vTarget);
						}
						hide();
					}
				}
			}

			if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
			{
				_clickedPos = _ptMouse;
			}
			if (KEYMANAGER->isOnceKeyUp(VK_RBUTTON))
			{
				if ((_clickedPos - vector2D(_ptMouse)).getLength() < 10)
				{
					_state = commandWindowState::Where;
				}				
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

			//메뉴 리스트 렌더링
			for (size_t i = 0; i < _menuList.size(); ++i)
			{
				RECT rc = RectMake(_renderPos.x, _renderPos.y + i * _menuHeight, _menuWidth, _menuHeight);
				IMAGEMANAGER->fillRectangle(rc, DefaultBrush::white);
				IMAGEMANAGER->drawRectangle(rc);
				IMAGEMANAGER->drawTextField(_renderPos.x, _renderPos.y + i * _menuHeight, UTIL::string_to_wstring(_menuList[i]), 14, _menuWidth, _menuHeight);
			}
			//선택 메뉴 렌더링
			if (_chooseIndex >= 0)
			{
				RECT rc = RectMake(_renderPos.x, _renderPos.y + _chooseIndex * _menuHeight, _menuWidth, _menuHeight);
				IMAGEMANAGER->fillRectangle(rc, D2D1::ColorF::LightSteelBlue, 0.5f);
			}
		}
		break;
	}
}


bool commandWindow::setMenuList()
{
	_menuList.clear();

	//유닛 찍음.
	if (_destUnit)
	{
		//아군을 찍었을 경우 => 대기, 자동, 원군
		if (_parent->getPlayerCountry()->getColor() == _destUnit->getCountryColor())
		{
			_menuList.push_back("원군");
		}
		//적 유닛을 찍었을 경우 => 대기, 자동, 추적
		else
		{
			_menuList.push_back("추적");
		}
	}
	//타일 찍음.
	else if (_destTile)
	{
		gameObject* objectOnTile = _destTile->getObjectOnTile();
		//오브젝트가 없는 땅인데 이동 가능할 경우 => 대기, 자동 + (건설 가능한 땅이면, 마을건축, 목책건축)
		if (!objectOnTile)
		{
			if (_destTile->isWalkable())
			{
				_menuList.push_back("대기");
				_menuList.push_back("자동");
				//대기, 자동 + (건설 가능한 땅이면, 마을건축, 목책건축)
				if (_destTile->isBuildable())
				{
					_menuList.push_back("마을 건축");
					_menuList.push_back("목책 건축");
				}
			}
			else
			{
				auto checkMovable = [this](int x, int y)
				{
					vector2D tileCount = WORLD->getMap()->getTileCount();

					// - 오버플로우 체크
					if (x < 0 || x >= tileCount.x || y < 0 || y >= tileCount.y)
					{
						return false;
					}
					// - 높이, 이동불가능 타일 체크
					terrainTile* checkTile = WORLD->getMap()->getTile(x, y);
					int distanceHeight = abs(checkTile->getHeight(true) - _destTile->getHeight());
					if (!checkTile->isWalkable() || distanceHeight > 1)
					{
						return false;
					}
					return true;
				};
				//다리 건설이 가능한지 확인
				//(x -1, +1에 이동 가능한 타일이 있을 경우) 또는 (y-1,+1에 이동 가능한 타일이 있을 경우)	
				vector2D tileIndex = _destTile->getIndex();
				if (checkMovable(tileIndex.x - 1, tileIndex.y) && checkMovable(tileIndex.x + 1, tileIndex.y))
				{
					_menuList.push_back("다리 건설");
				}
				else if (checkMovable(tileIndex.x, tileIndex.y - 1) && checkMovable(tileIndex.x, tileIndex.y + 1))
				{
					_menuList.push_back("다리 건설");
				}
			}
		}
		else
		{
			//오브젝트가 있는데 이동 가능한 오브젝트일 경우 => 대기, 자동, 파괴
			if (_destTile->isWalkable())
			{
				_menuList.push_back("대기");
				_menuList.push_back("자동");

				//다리일 경우 수리
				if (objectOnTile->_name == "다리")
				{
					_menuList.push_back("수리");
				}

				_menuList.push_back("파괴");
			}
			//오브젝트가 있는데 이동 불가능한 오브젝트일 경우 => 파괴
			else
			{
				//아군의 목책일 경우 수리
				if (objectOnTile->_name == "목책")
				{
					if(((mncObjectBase*)objectOnTile)->getCountryColor() == CountryColor::BLUE)
					{
						_menuList.push_back("수리");
					}
				}
				if (objectOnTile->_name != "돌")
					_menuList.push_back("파괴");
			}
		}
	}

	if (_menuList.size() > 0) return true;
	return false;
}










//=====================================================
// # 시스템 메시지

HRESULT systemMessage::init()
{
	gameObject::init("시스템메시지");

	_state = hide;
	_timer = 0;
	_displayTime = 0;
	_alpha = 0;

	return S_OK;
}

void systemMessage::release()
{
	gameObject::release();
}

void systemMessage::update()
{
	switch (_state)
	{
		case systemMessage::hide:
		break;
		case systemMessage::appear:
		{
			_alpha += 0.05f;
			if (_alpha >= 1.0f)
			{
				_alpha = 1.0f;
				_state = show; 
				_timer = 0;
			}
		}
		break;
		case systemMessage::show:
		{
			_timer += TIMEMANAGER->getElapsedTime();
			if (_timer >= _displayTime)
			{
				_state = disappear;
			}
		}
		break;
		case systemMessage::disappear:
		{
			_alpha -= 0.05f;
			if (_alpha <= 0)
			{
				_state = hide;
			}
		}
		break;
	}
}

void systemMessage::render()
{
	if (_state != hide)
	{
		RECT rc = RectMakeCenter(WINSIZEX*0.5f, WINSIZEY*0.5f, WINSIZEX, WINSIZEY*0.2f);
		float rectangleAlpha = _alpha;
		if (rectangleAlpha >= 0.5f) rectangleAlpha = 0.5f;
		IMAGEMANAGER->fillRectangle(rc, D2D1::ColorF::WhiteSmoke, rectangleAlpha);
		IMAGEMANAGER->drawTextField(rc.left, rc.top, UTIL::string_to_wstring(_text), _color, 30, 
			rc.right - rc.left, rc.bottom - rc.top, _alpha);
	}
}

void systemMessage::showMessage(string text, float displayTime, COLORREF color)
{
	if (_state != hide) return;

	_state = appear;
	_text = text;
	_displayTime = displayTime;
	_timer = 0;
	_color = color;
	_alpha = 0;
}
