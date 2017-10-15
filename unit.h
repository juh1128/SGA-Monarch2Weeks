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