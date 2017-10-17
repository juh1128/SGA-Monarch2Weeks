#pragma once
#include "gameObject.h"
#include <vector>

class unitState;

namespace UnitDirection
{
	enum DIRECTION
	{
		UNIT_LEFT,
		UNIT_UP,
		UNIT_RIGHT,
		UNIT_DOWN,
		UNIT_END
	};
}

namespace UnitState
{
	enum Enum
	{
		None, MoveOneStep, End
	};
}

class unit : public gameObject
{
private:
	vector2D _index;
	int		 _height;
	int		 _hp;
	bool	 _isStarUnit;

	UnitState::Enum _state;
	unitState*		_unitState;

	CountryColor::Enum _unitColor;

	float _moveSpeed;
	int _imageFrameX;

	float _livedTime;

	//�ڵ� �����ΰ�
	bool _isAuto;

	UnitDirection::DIRECTION _unitDirection;

	friend class unitNoneState;
	friend class unitOneStep;


public:
	unit();
	~unit();

	HRESULT init(vector2D index, int height,CountryColor::Enum country);
	void release();
	void update();
	void render();

	void moveCallBack(POINT directionTile,gameObject* dest);
	void changeState(unitState* newstate);
	void imageFrame(void);

	void build();
	void attack();
	void run(unit* _starUnit);

	vector2D getDirectionVector(UnitDirection::DIRECTION dir);

	CountryColor::Enum getCountryColor() { return _unitColor; }

	bool getIsStarUnit() { return _isStarUnit; }
	void setIsStarUnit(bool isStar) { _isStarUnit = isStar; }

	int getHealth() { return _hp; }
	void setHp(int num) { _hp = num; }

	void syncIndexFromPos();
	void requestRender();
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

	virtual void enter(unit& unit);
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