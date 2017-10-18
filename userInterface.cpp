#include "stdafx.h"
#include "userInterface.h"
#include "mncObjectBase.h"
#include "unit.h"

HRESULT userInterface::init()
{
	gameObject::init("�������̽�");

	_map = WORLD->getMap();
	return S_OK;
}

void userInterface::release()
{
	gameObject::release();
}

void userInterface::update()
{
	pickingTile();

	moveCamera();


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

void userInterface::pickingTile()
{
	//���콺 ��ŷ + ������ŷ �� WORLD ��ž
	_map->setMousePickTile();
	terrainTile* tile = _map->getPickedTile();
	if (tile)
	{
		if (tile->getUnitOnTile().size() > 0)
		{
			SCENEMANAGER->getNowScene()->sendMessage("disableWorld");
		}
		else
		{
			SCENEMANAGER->getNowScene()->sendMessage("enableWorld");
		}
	}
}


void userInterface::renderPickInfo()
{
	terrainTile* _pickedTile = _map->getPickedTile();

	//��ŷ ������Ʈ ���� ǥ��
	if (_pickedTile)
	{
		vector2D tilePos = CAMERA->getRelativeVector2D(_pickedTile->_pos);
		tilePos.y -= _pickedTile->getHeight()*_map->getTileSize().y*0.5f;
		tilePos.y += _map->getTileSize().y - 10;

		//������ ���� ��� ���� ���� ǥ��
		vector<unit*> unitList = _pickedTile->getUnitOnTile();
		if (unitList.size() > 0)
		{
			char buf[128] = "";
			wsprintf(buf, "[%s] DF %d", unitList[0]->_name.c_str(), unitList[0]->getHealth());
			IMAGEMANAGER->fillRectangle(RectMakeCenter(tilePos.x + 12, tilePos.y + 16, strlen(buf) * 10, 24), D2D1::ColorF::FloralWhite, 0.7f);
			IMAGEMANAGER->drawText(tilePos.x - strlen(buf) * 9, tilePos.y, UTIL::string_to_wstring(buf), 20, DefaultBrush::black);
		}

		//������Ʈ�� ���� ��� ������Ʈ�� ���� ǥ��
		else if (_pickedTile->getObjectOnTile())
		{
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
}
