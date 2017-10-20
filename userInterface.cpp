#include "stdafx.h"
#include "userInterface.h"
#include "mncObjectBase.h"
#include "unit.h"

HRESULT userInterface::init(CountryColor::Enum playerColor)
{
	gameObject::init("�������̽�");

	//�������̽� ���ҽ� �ε�
	IMAGEMANAGER->addImage("interfaceBack", L"resource/interface/myInterfaceBack.png");
	IMAGEMANAGER->addFrameImage("startBtn", L"resource/interface/startButton.png", 2, 1);
	IMAGEMANAGER->addFrameImage("taxProgress", L"resource/interface/taxProgress.png", 1, 2);
	_otherCountryInfo = IMAGEMANAGER->addImage("otherCountry", L"resource/interface/otherCountry.png");
	_countryColorSprite = IMAGEMANAGER->addFrameImage("countryColor", L"resource/interface/countryColor.png", 4, 1);

	_pickedUnit = NULL;
	_map = WORLD->getMap();
	_playerCountry = WORLD->getCountry(playerColor);

	//�������̽� ��ü ����
	_back = new interfaceBack;
	_back->init();
	_startBtn = new startButton;
	_startBtn->init(_back);
	_taxProgress = new taxProgress;
	_taxProgress->init(_back, _playerCountry->getTaxRate());

	_commandWindow = new commandWindow;
	_commandWindow->init();

	return S_OK;
}

void userInterface::release()
{
	gameObject::release();
}

void userInterface::update()
{
	_map->setMousePickTile();   //Ÿ�� ��ŷ
	pickUnit();					//���� ��ŷ

	moveCamera();				//ī�޶� �̵�


	//�������̽� ������Ʈ
	_taxProgress->update();
	_startBtn->update();

	//��� �������̽�
	clickedMouse();
	dragedMouse();
	_commandWindow->update();

	//����ȭ
	_playerCountry->setTaxRate(_taxProgress->getTaxRate());
}

void userInterface::render()
{
	//��ŷ ���� ������
	renderPickInfo();

	//���� ���� ������
	renderCountryInfo();

	//��� �������̽� ������
	_commandWindow->render();
}

void userInterface::moveCamera()
{
	//ī�޶� �̵� ����
	if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
	{
		_rbDownPos = _ptMouse;
	}
	if (KEYMANAGER->isStayKeyDown(VK_RBUTTON))
	{
		CAMERA->setPos(CAMERA->getPos() + _rbDownPos - vector2D(_ptMouse));
		_rbDownPos = _ptMouse;
	}
}

void userInterface::pickUnit()
{
	//���콺�� �ִ� Ÿ��
	terrainTile* tile = _map->getPickedTile();
	if (!tile) return;

	terrainTile* tiles[9];
	_map->get8Tiles(tiles, tile->getIndex().x, tile->getIndex().y);
	tiles[8] = tiles[0];
	tiles[0] = tile;

	//���콺�� �ִ� Ÿ�� ���� 8�� Ÿ���� ���ְ� �浹üũ
	vector<unit*> unitList;
	float zoom = CAMERA->getZoom();
	vector2D mousePos = CAMERA->getMousePos();
	_pickedUnit = NULL;

	for (int i = 0; i < 9; ++i)
	{
		if (tiles[i])
		{
			unitList = tiles[i]->getUnitOnTile();
			if (unitList.size() > 0)
			{
				//���콺�� �浹üũ
				for (size_t j = 0; j < unitList.size(); ++j)
				{
					if (!unitList[j]->isLive()) continue;

					vector2D pos = unitList[j]->_pos * zoom;
					vector2D size = unitList[j]->getSize() * zoom;
					RECT rc = RectMakeCenter(pos.x, pos.y, size.x, size.y);

					if (PtInRect(&rc, mousePos.toPoint()))
					{
						_pickedUnit = unitList[j];
						break;
					}
				}
				unitList.clear();
			}		
		}
	}
}

void userInterface::renderPickInfo()
{
	terrainTile* _pickedTile = _map->getPickedTile();
	if (!_pickedTile) return;

	//��ŷ ���� ���� ǥ��
	if (_pickedUnit)
	{
		if (_pickedUnit->isLive())
		{
			float zoom = CAMERA->getZoom();
			vector2D unitPos = CAMERA->getRelativeVector2D(_pickedUnit->_pos*zoom);
			unitPos.y += _pickedUnit->getSize().y*zoom*0.5f;

			char buf[128] = "";
			wsprintf(buf, "[%s] DF %d", _pickedUnit->_name.c_str(), _pickedUnit->getHealth());
			IMAGEMANAGER->fillRectangle(RectMakeCenter(unitPos.x + 12, unitPos.y + 16, strlen(buf) * 10, 24), D2D1::ColorF::FloralWhite, 0.7f);
			IMAGEMANAGER->drawText(unitPos.x - strlen(buf) * 9, unitPos.y, UTIL::string_to_wstring(buf), 20, DefaultBrush::black);
			return;
		}
	}

	//��ŷ ������Ʈ ���� ǥ��
	mncObjectBase* pickedObj = (mncObjectBase*)_pickedTile->getObjectOnTile();
	if (pickedObj)
	{
		if (pickedObj->isLive())
		{
			vector2D tilePos = CAMERA->getRelativeVector2D(_pickedTile->_pos);
			tilePos.y -= _pickedTile->getHeight()*_map->getTileSize().y*0.5f;
			tilePos.y += _map->getTileSize().y - 10;

			char buf[128] = "";
			if (pickedObj->_name != "��")
			{
				wsprintf(buf, "[%s] DF %d", pickedObj->_name.c_str(), pickedObj->getHp());
				IMAGEMANAGER->fillRectangle(RectMake(tilePos.x - strlen(buf) * 5 - 8, tilePos.y + 4, strlen(buf) * 10, 24), D2D1::ColorF::FloralWhite, 0.7f);
				IMAGEMANAGER->drawText(tilePos.x - strlen(buf) * 9, tilePos.y, UTIL::string_to_wstring(buf), 20, DefaultBrush::black);
			}
		}
	}
}

void userInterface::renderCountryInfo()
{
	//�������̽� ������
	_back->render();
	_startBtn->render();

	_taxProgress->render();

	//�ٸ� ����
	char buf[128] = "";
	vector2D otherCountryInfoPos = vector2D(_back->_pos.x, _back->_pos.y + _back->getSize().y);
	_otherCountryInfo->render(otherCountryInfoPos.x, otherCountryInfoPos.y, Pivot::LEFT_TOP, false);

	auto getCountryName = [](CountryColor::Enum color) {
		switch (color)
		{
			case CountryColor::RED:
				return "�޵�";
			break;
			case CountryColor::WHITE:
				return "�α׷�";
			break;
			case CountryColor::BLUE:
				return "��Ʈ����";
			break;
			case CountryColor::GREEN:
				return "�˺�";
			break;
		}
	};

	otherCountryInfoPos.x += 12;
	otherCountryInfoPos.y += 10;
	int index = 0;
	for (int i = 0; i < CountryColor::END-1; ++i)
	{
		country* cty = WORLD->getCountry((CountryColor::Enum)i);
		if (cty == _playerCountry) continue;

		_countryColorSprite->frameRender(otherCountryInfoPos.x, otherCountryInfoPos.y + index *30,
			i, 0, Pivot::LEFT_TOP, false);

		wsprintf(buf, "[%s] ������: %dG ������: %d", getCountryName((CountryColor::Enum)i),
			cty->getGold(), cty->getCountryPower());	
		IMAGEMANAGER->drawText(otherCountryInfoPos.x+30, otherCountryInfoPos.y + index * 30, 
			UTIL::string_to_wstring(buf), 12, DefaultBrush::black,
			DWRITE_TEXT_ALIGNMENT_LEADING);

		index++;
	}

	//�ڱ� ǥ��
	_countryColorSprite->frameRender(_back->_pos.x + 64, _back->_pos.y + 40,
		_playerCountry->getColor(), 0, Pivot::LEFT_TOP, false);

	wsprintf(buf, "[%s] ������: %dG ������: %d", getCountryName(_playerCountry->getColor()), _playerCountry->getGold(),
		_playerCountry->getCountryPower());
	IMAGEMANAGER->drawText(_back->_pos.x + 84, _back->_pos.y + 40, UTIL::string_to_wstring(buf), 11, DefaultBrush::black,
		DWRITE_TEXT_ALIGNMENT_LEADING);
}




void userInterface::clickedMouse()
{
	if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON))
	{
		if (_pickedUnit)
		{
			_commandWindow->show(_pickedUnit);
		}
	}
}

void userInterface::dragedMouse()
{
}