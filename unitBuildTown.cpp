#include "stdafx.h"
#include "unit.h"
#include "objectFactory.h"
#include "mncObjectBase.h"


void unitBuildTown::enter(unit& me)
{
	me._state = UnitState::BuildTown;
	_frameTimer = 0;
	//������ ������ ���� �� ����.
	objectFactory factory;
	WORLD->addObject(factory.createObject(_destIndex.x, _destIndex.y, me.getColorString() + "Town"));
	_obj = (mncObjectBase*)WORLD->getMap()->getTile(_destIndex.x, _destIndex.y)->getObjectOnTile();
	_obj->setHp(1);
}

void unitBuildTown::update(unit& me)
{
	_frameTimer++;		
	
	int health = me.getHealth();
	int objHp = _obj->getHp();

	if (_frameTimer % 2 == 0)
	{
		objHp += 1;
		_obj->setHp(objHp);
		health -= 1;
		me.setHp(health);

		_frameTimer = 0;
	}	

	if (objHp >= 50)
	{
		cout << "��i" << endl;
		return me.changeState(new unitNoneState);
	}
}

bool unit::isBuildableTown(POINT index)
{
	//�ڱ� ü���� 50���� ���ƾ� ���� �� ����.
	if (_hp <= 50) return false;

	//index �����÷ο�, ���� üũ, �� ������ �ִ��� üũ	
	if (!isMoveable(index)) return false;
		
	//�Ǽ� �������� üũ
	terrainTile* tile = WORLD->getMap()->getTile(index.x, index.y);
	if (!tile->isBuildable()) return false;
	
	//�Ǽ��Ϸ��� �� �ֺ��� 8Ÿ���� �����ͼ� �Ʊ� ������ �ִ��� üũ
	//�Ʊ� ������ �̹� �Ǽ��Ǿ��ִٸ� �Ǽ� ����.
	terrainTile* tiles[8];
	WORLD->getMap()->get8Tiles(tiles, index.x, index.y);
	for (int i = 0; i < 8; ++i)
	{
		if (tiles[i])
		{
			mncObjectBase* objectOnTile = (mncObjectBase*)(tiles[i]->getObjectOnTile());
			if (objectOnTile != NULL)
			{
				if (objectOnTile->isLive())
				{
					if (objectOnTile->_name == "����" && objectOnTile->getCountryColor() == _unitColor)
					{
						return false;
						break;
					}
				}
			}
		}
	}

	return true;
}