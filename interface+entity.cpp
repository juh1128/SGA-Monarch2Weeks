#include "stdafx.h"
#include "userInterface.h"
#include "unit.h"
#include "mncObjectBase.h"



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
//Ŭ�� ��
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
//�巡�� ��
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
		//���?
		case commandWindowState::Where:
		{
			//Ÿ�� Ŭ�� �� What���� �Ѿ.
			if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
			{
				_destTile = WORLD->getMap()->getPickedTile();
				_destUnit = _parent->getPickedUnit();
				if (_destUnit)
				{
					if (_destUnit->isLive())
					{
						_state = commandWindowState::What;
						setMenuList();	//�� �������Ŀ� ���� �޴��� �����Ѵ�.
						_renderPos = _ptMouse;
					}
				}
				else if (_destTile)
				{
					_state = commandWindowState::What;
					setMenuList();	
					_renderPos = _ptMouse;
				}				
			}

			if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
			{
				hide();
			}
		}
		break;
		//��?
		case commandWindowState::What:
		{
			//�޴� �浹üũ
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

			if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
			{
				_state = commandWindowState::Where;
			}
		}
		break;
	}

}

void commandWindow::setMenuList()
{
	_menuList.clear();

	//���� ����.
	if (_destUnit)
	{
		//�Ʊ��� ����� ��� => ���, �ڵ�, ����
		if (_parent->getPlayerCountry()->getColor() == _destUnit->getCountryColor())
		{
			_menuList.push_back("���");
			_menuList.push_back("�ڵ�");
			_menuList.push_back("����");
		}
		//�� ������ ����� ��� => ���, �ڵ�, ����
		else
		{
			_menuList.push_back("���");
			_menuList.push_back("�ڵ�");
			_menuList.push_back("����");
		}
	}
	//Ÿ�� ����.
	else if (_destTile)
	{
		gameObject* objectOnTile = _destTile->getObjectOnTile();
		//������Ʈ�� ���� ���ε� �̵� ������ ��� => ���, �ڵ� + (�Ǽ� ������ ���̸�, ��������, ��å����)
		if (!objectOnTile)
		{
			if (_destTile->isWalkable())
			{
				_menuList.push_back("���");
				_menuList.push_back("�ڵ�");
				//���, �ڵ� + (�Ǽ� ������ ���̸�, ��������, ��å����)
				if (_destTile->isBuildable())
				{
					_menuList.push_back("��������");
					_menuList.push_back("��å����");
				}
			}
			else
			{
				//�ٸ� �Ǽ��� �������� Ȯ��
			}
		}
		else
		{
			//������Ʈ�� �ִµ� �̵� ������ ������Ʈ�� ��� => ���, �ڵ�, �ı�
			if (_destTile->isWalkable())
			{
				_menuList.push_back("���");
				_menuList.push_back("�ڵ�");
				_menuList.push_back("�ı�");
			}
			//������Ʈ�� �ִµ� �̵� �Ұ����� ������Ʈ�� ��� => �ı�
			else
			{
				_menuList.push_back("�ı�");
			}
		}	
	}
}

void commandWindow::render()
{
	float zoom = CAMERA->getZoom();
	unit* target = NULL;
	vector2D renderPos;

	switch (_state)
	{
		//���?
		case commandWindowState::Where:
		{
			//��ǳ�� ������
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
		//��?
		case commandWindowState::What:
		{
			//��ǳ�� ������
			for (size_t i = 0; i < _targetList.size(); ++i)
			{
				target = _targetList[i];
				renderPos = target->_pos * zoom;
				float yOffset = target->getSize().y*zoom*0.5f;
				_whatwhereImage->setScaleOption(vector2D(zoom, zoom));
				_whatwhereImage->frameRender(renderPos.x, renderPos.y - yOffset, 0, 0, Pivot::BOTTOM);
			}

			//�޴� ����Ʈ ������
			for (size_t i = 0; i < _menuList.size(); ++i)
			{
				RECT rc = RectMake(_renderPos.x, _renderPos.y + i * _menuHeight, _menuWidth, _menuHeight);
				IMAGEMANAGER->fillRectangle(rc, DefaultBrush::white);
				IMAGEMANAGER->drawRectangle(rc);
				IMAGEMANAGER->drawTextField(_renderPos.x, _renderPos.y + i * _menuHeight, UTIL::string_to_wstring(_menuList[i]), 14, _menuWidth, _menuHeight);
			}
			//���� �޴� ������
			if (_chooseIndex >= 0)
			{
				RECT rc = RectMake(_renderPos.x, _renderPos.y + _chooseIndex * _menuHeight, _menuWidth, _menuHeight);
				IMAGEMANAGER->fillRectangle(rc, D2D1::ColorF::LightSteelBlue, 0.5f);
			}
		}
		break;
	}
}
