#pragma once
#include "gameObject.h"
#include <vector>
//static vector2D up(2, -1);
//static vector2D down(-2, 1);
//static vector2D right(2,1);
//static vector2D left(-2,-1);

class unitState;

class unit : public gameObject
{
private:
	vector2D _index;

	unitState* _unitState;

	float _moveSpeed;

	friend class unitNoneState;
	friend class unitOneStep;


public:
	unit();
	~unit();

	HRESULT init();
	void release();
	void update();
	void render();

	void moveCallBack(gameObject* dset);
	void changeState(unitState* newstate);
};

class unitState
{
private:
public:
	virtual ~unitState() {}

	virtual void enter(unit& unit) {}
	virtual void update(unit& unit) {}
};

class unitNoneState : public unitState
{
private:
public:

	virtual void enter(unit& unit) {}
	virtual void update(unit& unit);

};

class unitOneStep : public unitState
{
private:
	vector2D _destPos;
	terrainTile* _destTile;
	int _destheight;
	float _moveRatio;
public:
	unitOneStep(terrainTile* tile)
	{
		_destTile = tile;
	}
	virtual void enter(unit& unit);
	virtual void update(unit& unit);

};