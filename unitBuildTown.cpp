#include "stdafx.h"
#include "unit.h"
#include "objectFactory.h"
#include "mncObjectBase.h"


void unitBuildTown::enter(unit& me)
{
	me._state = UnitState::BuildTown;

	//������ ������ ���� �� ����.
	objectFactory factory;
	WORLD->addObject(factory.createObject(_destIndex.x, _destIndex.y, me.getColorString() + "Town"));
	me._hp -= 25;

	me.changeState(new unitNoneState);
}

void unitBuildTown::update(unit& me)
{

}

bool unit::isBuildableTown(POINT index)
{
	//�ڱ� ü���� 25���� ���ƾ� ���� �� ����.
	if (_hp <= 25) return false;

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