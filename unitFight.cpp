#include "stdafx.h"
#include "unit.h"
#include <algorithm>

unit* unit::isCanAttack()
{
	vector<unit*> enemy;
	for (int i = 0; i < 4; i++)
	{
		vector2D direct = _index + getDirectionVector((UnitDirection::DIRECTION)i);
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
	health = health*health - emHealth*emHealth;
	me.setHp(sqrt(health));
	cout << "싸우는중" << endl;
	if (me.getHealth() <= 0)
	{
		cout << "피없당" << endl;
		return;
	}
}
