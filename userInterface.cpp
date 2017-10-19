#include "stdafx.h"
#include "userInterface.h"
#include "mncObjectBase.h"
#include "unit.h"

HRESULT userInterface::init()
{
	gameObject::init("�������̽�");

	_pickedUnit = NULL;
	_map = WORLD->getMap();
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

}

void userInterface::render()
{
	renderPickInfo();
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

	if (_pickedUnit)
	{
		SCENEMANAGER->getNowScene()->sendMessage("disableWorld");
	}
	else
	{
		SCENEMANAGER->getNowScene()->sendMessage("enableWorld");
	}
}

void userInterface::renderPickInfo()
{
	terrainTile* _pickedTile = _map->getPickedTile();
	if (!_pickedTile) return;

	//��ŷ ���� ���� ǥ��
	if (_pickedUnit)
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

	//��ŷ ������Ʈ ���� ǥ��
	if (_pickedTile->getObjectOnTile())
	{
		vector2D tilePos = CAMERA->getRelativeVector2D(_pickedTile->_pos);
		tilePos.y -= _pickedTile->getHeight()*_map->getTileSize().y*0.5f;
		tilePos.y += _map->getTileSize().y - 10;

		char buf[128] = "";
		mncObjectBase* obj = (mncObjectBase*)_pickedTile->getObjectOnTile();
		if (obj->_name != "��")
		{
			wsprintf(buf, "[%s] DF %d", obj->_name.c_str(), obj->getHp());
			IMAGEMANAGER->fillRectangle(RectMake(tilePos.x - strlen(buf) * 5 - 8, tilePos.y + 4, strlen(buf) * 10, 24), D2D1::ColorF::FloralWhite, 0.7f);
			IMAGEMANAGER->drawText(tilePos.x - strlen(buf) * 9, tilePos.y, UTIL::string_to_wstring(buf), 20, DefaultBrush::black);
		}
	}
}
