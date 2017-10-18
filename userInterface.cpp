#include "stdafx.h"
#include "userInterface.h"
#include "mncObjectBase.h"
#include "unit.h"

HRESULT userInterface::init()
{
	gameObject::init("인터페이스");

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
	//카메라 이동 구현
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
	//마우스 피킹 + 유닛피킹 시 WORLD 스탑
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

	//피킹 오브젝트 정보 표시
	if (_pickedTile)
	{
		vector2D tilePos = CAMERA->getRelativeVector2D(_pickedTile->_pos);
		tilePos.y -= _pickedTile->getHeight()*_map->getTileSize().y*0.5f;
		tilePos.y += _map->getTileSize().y - 10;

		//유닛이 있을 경우 유닛 정보 표시
		vector<unit*> unitList = _pickedTile->getUnitOnTile();
		if (unitList.size() > 0)
		{
			char buf[128] = "";
			wsprintf(buf, "[%s] DF %d", unitList[0]->_name.c_str(), unitList[0]->getHealth());
			IMAGEMANAGER->fillRectangle(RectMakeCenter(tilePos.x + 12, tilePos.y + 16, strlen(buf) * 10, 24), D2D1::ColorF::FloralWhite, 0.7f);
			IMAGEMANAGER->drawText(tilePos.x - strlen(buf) * 9, tilePos.y, UTIL::string_to_wstring(buf), 20, DefaultBrush::black);
		}

		//오브젝트가 있을 경우 오브젝트의 정보 표시
		else if (_pickedTile->getObjectOnTile())
		{
			char buf[128] = "";
			mncObjectBase* obj = (mncObjectBase*)_pickedTile->getObjectOnTile();
			if (obj->_name != "돌")
			{
				wsprintf(buf, "[%s] DF %d", obj->_name.c_str(), obj->getHp());
				IMAGEMANAGER->fillRectangle(RectMake(tilePos.x - strlen(buf) * 5 - 8, tilePos.y + 4, strlen(buf) * 10, 24), D2D1::ColorF::FloralWhite, 0.7f);
				IMAGEMANAGER->drawText(tilePos.x - strlen(buf) * 9, tilePos.y, UTIL::string_to_wstring(buf), 20, DefaultBrush::black);
			}
		}
	}
}
