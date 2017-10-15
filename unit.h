#pragma once
#include "gameObject.h"
#include <vector>

class unitState;

namespace UnitDirection
{
	enum DIRECTION
	{
		UNIT_UP,
		UNIT_DOWN,
		UNIT_RIGHT,
		UNIT_LEFT,
		UNIT_END
	};

}
class unit : public gameObject
{
private:
	vector2D _index;

	unitState* _unitState;

	float _moveSpeed;
	int _imageFrameX;
	int _imageFrameY;

	float _livedTime;

	friend class unitNoneState;
	friend class unitOneStep;


public:
	unit();
	~unit();

	HRESULT init();
	void release();
	void update();
	void render();

	void moveCallBack(POINT directionTile,gameObject* dest);
	void changeState(unitState* newstate);
	void imageFrame(void);
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
	vector2D _directionIndex;
	vector2D _oldIndex;
	int _destheight;
	float _moveRatio;
	float _zoom;

public:
	unitOneStep(terrainTile* tile,POINT direction)
	{
		_destTile = tile;
		_directionIndex.x = direction.x;
		_directionIndex.y = direction.y;
	}
	virtual void enter(unit& unit);
	virtual void update(unit& unit);
	void unitdirection(unit& unit);
};