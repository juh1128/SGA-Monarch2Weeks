#include "stdafx.h"
#include "unit.h"
#include <algorithm>

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
void unitFight::enter(unit & me)
{

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
	}
	else if (health == emHealth)
	{
		health -= _enemyUnit->getHealth()*0.002f;
		emHealth -= me.getHealth()*0.002f;
	}
	else if (health < emHealth)
	{
		health -= _enemyUnit->getHealth()*0.002f;
		emHealth -= me.getHealth()*0.001f;
	}

	me.setHp(health);
	_enemyUnit->setHp(emHealth);

	cout << "싸우는중" << endl;
	if (me.getHealth() <= 0)
	{
		cout << "피없당" << endl;
		return;
	}
}
