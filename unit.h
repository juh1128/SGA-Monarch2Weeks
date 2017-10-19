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
		CreateMotion, None, Run, BuildTown, End
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

	float _frameTimer;

	//자동 상태인가
	bool _isAuto;
	bool _isMove;



	UnitDirection::DIRECTION _unitDirection;

	friend class unitCreateMotion;
	friend class unitNoneState;
	friend class unitOneStep;
	friend class unitBuildTown;
	friend class unitRun;
	friend class unitFight;
public:
	unit();
	~unit();

	HRESULT init(vector2D index, int height,CountryColor::Enum country);
	void release();
	void update();
	void render();

	void changeState(unitState* newstate);
	void imageFrame(void);

	vector2D getDirectionVector(UnitDirection::DIRECTION dir);

	CountryColor::Enum getCountryColor() { return _unitColor; }

	bool getIsStarUnit() { return _isStarUnit; }
	void setIsStarUnit(bool isStar) { _isStarUnit = isStar; }

	int getHealth() { return _hp; }
	void setHp(int num) { _hp = num;  if (_hp < 0) _hp = 0; }

	void syncIndexFromPos();
	void requestRender();

	string getColorString();

	bool isMoveable(POINT index);
	bool isBuildableTown(POINT index);
	unit* isCanRun();
	unit* isCanAttack();
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
	virtual void enter(unit& me);
	virtual void update(unit& me);

	void moveOneStep(unit& me);


};

class unitOneStep : public unitState
{
private:
	terrainTile* _destTile;

	vector2D _destIndex;
	vector2D _oldIndex;

public:
	unitOneStep(int xIndex, int yIndex)
	{
		_destIndex.x = xIndex;
		_destIndex.y = yIndex;
	}
	virtual void enter(unit& unit);
	virtual void update(unit& unit);
};

class unitBuildTown : public unitState
{
private:
	POINT _destIndex;
public:
	unitBuildTown(POINT index) { _destIndex = index; }
	virtual void enter(unit& me);
	virtual void update(unit& me);
};

class unitCreateMotion : public unitState
{
private:
	//생성 연출
	float _createFrameTimer;
	int _rotateNum;

public:
	unitCreateMotion() {}
	virtual void enter(unit& me);
	virtual void update(unit& me);
};

class unitRun : public unitState
{
private:
	unit* _avoidUnit;
public:
	unitRun(unit* avoidUnit)
	{
		_avoidUnit = avoidUnit;
	}
	virtual void enter(unit& me);
	virtual void update(unit& me) {}
};

class unitFight : public unitState
{
private:
	unit* _enemyUnit;
public:
	unitFight() {}
	unitFight(unit* enemy)
	{
		_enemyUnit = enemy;
	}

	virtual void enter(unit& me);
	virtual void update(unit& me);
};