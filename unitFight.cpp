#include "stdafx.h"
#include "unit.h"
#include <algorithm>
#include "mncObjectBase.h"

unit* unit::isCanAttack()
{
	vector<unit*> enemy;
	for (int i = 0; i < 4; i++)
	{
		vector2D direct = _index + getDirectionVector((UnitDirection::DIRECTION)i);
		
		if (direct.x <0 || direct.x > WORLD->getMap()->getTileCount().x - 1) continue;
		if (direct.y <0 || direct.y > WORLD->getMap()->getTileCount().y - 1) continue;

		if (abs(WORLD->getMap()->getTile(_index.x, _index.y)->getHeight() - WORLD->getMap()->getTile(direct.x, direct.y)->getHeight()) > 1)
			continue;
		vector<unit*> emUnit = WORLD->getMap()->getTile(direct.x, direct.y)->getUnitOnTile();


		if (emUnit.size() <= 0) continue;

		for (int i = 0; i < emUnit.size(); ++i)
		{
			if (!emUnit[i]->isLive()) continue;
			if (this->getCountryColor() != emUnit[i]->getCountryColor())
				enemy.push_back(emUnit[i]);
		}

		emUnit.clear();
	}
	
	if (enemy.size() <= 0) return NULL;

	sort(enemy.begin(), enemy.end(), [](unit* a, unit* b)
	{
		if (a->getHealth() < b->getHealth())
			return true;
		return false;
	});

	if(this->getCountryColor() == CountryColor::BLUE)
		cout << _index.x <<" , "<< _index.y << enemy[0]->getColorString() << endl;

	vector2D direction = enemy[0]->_index - _index;

	int length = direction.getLength();

	if (length> 1 || length == 0)
	{
		return nullptr;
	}

	return enemy[0];
}
mncObjectBase* unit::isCanAttackNature()
{
	vector<mncObjectBase*> nature;
	tileMap* map = WORLD->getMap();
	vector2D tileCount = map->getTileCount();

	if (_commandStateName == "�ı�")
	{
		mncObjectBase* comobj = (mncObjectBase*)_commandDestTile->getObjectOnTile();
		if (comobj)
		{
			return comobj;
		}
	}

	//�ڱ� �ֺ� 4���⿡�� �ı� �� �� �ִ� ������Ʈ�� �ִ��� üũ�Ѵ�.
	for (int i = 0; i < 4; ++i)
	{
		vector2D direction = _index + getDirectionVector(UnitDirection::DIRECTION(i));
		//�ε��� �����÷ο� üũ
		if (direction.x < 0 || direction.x >= tileCount.x || direction.y < 0 || direction.y >= tileCount.y)
			continue;

		terrainTile* tile = map->getTile(direction.x, direction.y);
		//�ش� Ÿ�ϰ��� ���� üũ
		if (abs(tile->getHeight() - _height) > 1) continue;

		gameObject* onObject = tile->getObjectOnTile();
		//�ı� �� �� �ִ� ������Ʈ�� �ִ���?
		if (onObject)
		{
			if (onObject->isLive())
			{
				mncObjectBase* obj = (mncObjectBase*)onObject;

				// - �Ʊ��� �ǹ��̸� ����
				if (obj->getCountryColor() == _unitColor)continue;
				// - ���� �μ� �� ����.
				if (onObject->_name == "��") continue;

				if (onObject->_name == "�ٸ�") continue;

				nature.push_back(obj);
			}
		}
	}

	if (nature.size() > 0)
	{
		//�ı� ������ ������Ʈ�� ������, �������� ��� ��ȯ�Ѵ�.
		return nature[RND->getFromIntTo(0, nature.size() - 1)];
	}
	return NULL;
}

void unitFight::enter(unit & me)
{
	me._state = UnitState::Fight;

	vector2D direction = _enemyUnit->_index - me._index;

	int length = direction.getLength();

	if (length> 1 || length == 0)
	{
		return me.changeState(new unitNoneState);
	}

	if (direction.x == 1)
		me._unitDirection = UnitDirection::UNIT_RIGHT;
	else if (direction.x == -1)
		me._unitDirection = UnitDirection::UNIT_LEFT;
	else if (direction.y == 1)
		me._unitDirection = UnitDirection::UNIT_DOWN;
	else if (direction.y == -1)
		me._unitDirection = UnitDirection::UNIT_UP;
	else
		cout << direction.x <<" , "<< direction.y <<"unitFight enter ���⼳�� ����" << endl;

	//if (_enemyUnit->_commandStateName != "")
	//	_enemyUnit->resetCommand();
	
}

void unitFight::update(unit & me)
{
	_frameTimer++;

	int health = me.getHealth();
	int emHealth = _enemyUnit->getHealth();

	if (emHealth <= 0) return me.changeState(new unitNoneState);
	
	if (_frameTimer % 4 == 0)
	{

		if (health > emHealth)
		{
			health -= _enemyUnit->getHealth()*0.001f;
			emHealth -= me.getHealth()*0.002f;

			_enemyUnit->setHp(emHealth);
			if (_enemyUnit->getHealth() <= 0) return me.changeState(new unitNoneState);
			me.setHp(health);
		}
		else if (health == emHealth)
		{
			health -= _enemyUnit->getHealth()*0.002f;
			emHealth -= me.getHealth()*0.002f;

			me.setHp(health);
			_enemyUnit->setHp(emHealth);

		}
		else if (health < emHealth)
		{
			health -= _enemyUnit->getHealth()*0.002f;
			emHealth -= me.getHealth()*0.001f;

			me.setHp(health);
			if (me.getHealth() <= 0) return me.changeState(new unitNoneState);
			_enemyUnit->setHp(emHealth);

		}
		_frameTimer = 0;
	}
}
void unitDigObject::enter(unit& me)
{
	me._state = UnitState::Destroy;

	//if (_nature->_name == "����")
	//{
		vector2D direction = _nature->getIndex() - me._index;

		int length = direction.getLength();
		if (length> 1 || length == 0)
		{
			return me.changeState(new unitNoneState);
		}

		if (direction.x == 1)
			me._unitDirection = UnitDirection::UNIT_RIGHT;
		else if (direction.x == -1)
			me._unitDirection = UnitDirection::UNIT_LEFT;
		else if (direction.y == 1)
			me._unitDirection = UnitDirection::UNIT_DOWN;
		else if (direction.y == -1)
			me._unitDirection = UnitDirection::UNIT_UP;
		else
			cout << direction.x << " , " << direction.y << "unitDigObject enter ���⼳�� ����" << endl;

	//}

}
void unitDigObject::update(unit& me)
{
	_frameTimer++;

	int health = me.getHealth();
	int natureHP = _nature->getHp();

	if (natureHP <= 0) return me.changeState(new unitNoneState);

	if (_frameTimer % 4 == 0)
	{
		natureHP -= me.getHealth()*0.03f * 0.2f;
		health -= _nature->getHp()*0.05f;

		me.setHp(health);
		_nature->setHp(natureHP);

		_frameTimer = 0;
	}
}