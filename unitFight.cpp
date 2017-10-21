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

		vector<unit*> emUnit = WORLD->getMap()->getTile(direct.x, direct.y)->getUnitOnTile();

		if (emUnit.size() <= 0) continue;

		for (int i = 0; i < emUnit.size(); ++i)
		{
			if (!emUnit[i]->isLive()) continue;
			if (this->getCountryColor() != emUnit[i]->getCountryColor())
				enemy.push_back(emUnit[i]);
		}
	}
	if (enemy.size() <= 0) return NULL;

	sort(enemy.begin(), enemy.end(), [](unit* a, unit* b)
	{
		if (a->getHealth() < b->getHealth())
			return true;
		return false;
	});

	cout << "적있당" << endl;

	return enemy[0];
}
mncObjectBase* unit::isCanAttackNature()
{
	if (this->getCountryColor() != CountryColor::BLUE) return nullptr ;

	vector<mncObjectBase*> nature;
	vector2D direct = _index + getDirectionVector(_unitDirection);

	if (direct.x <0 || direct.x > WORLD->getMap()->getTileCount().x - 1) return nullptr;
	if (direct.y <0 || direct.y > WORLD->getMap()->getTileCount().y - 1) return nullptr;

	mncObjectBase* obj = (mncObjectBase*)WORLD->getMap()->getTile(direct.x, direct.y)->getObjectOnTile();
	if(obj)
		nature.push_back(obj);

	if (nature.size() <= 0) return nullptr;
	if (nature[0]->getCountryColor() == this->getCountryColor()) return nullptr;
	if (nature[0]->_name == "돌") return nullptr;

	return nature[0];


	cout << "isCanAttackNature 버그 있어욤" << endl;

	return nullptr;
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
		cout << direction.x <<" , "<< direction.y <<"unitFight enter 방향설정 오류" << endl;
}

void unitFight::update(unit & me)
{
	
	int health = me.getHealth();
	int emHealth = _enemyUnit->getHealth();

	if (emHealth <= 0) return me.changeState(new unitNoneState);
	
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


}
void unitDigObject::enter(unit& me)
{
	me._state = UnitState::Fight;
}
void unitDigObject::update(unit& me)
{
	int health = me.getHealth();
	int natureHP = _nature->getHp();

	if (natureHP <= 0) return me.changeState(new unitNoneState);

	natureHP -= me.getHealth()*0.03f * 0.2f;
	health -= _nature->getHp()*0.05f;

	me.setHp(health);
	_nature->setHp(natureHP);
}