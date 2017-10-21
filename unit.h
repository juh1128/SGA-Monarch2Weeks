#pragma once
#include "gameObject.h"
#include <vector>
#include <deque>

class unitState;
class mncObjectBase;

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
		Stop, Search, CreateMotion, Fight, Run, BuildTown, Merge, End
	};
}

class unit : public gameObject
{
private:
	vector2D _index;
	int		 _height;
	int		 _hp;
	bool	 _isStarUnit;

	UnitState::Enum		_state;
	unitState*			_unitState;
	vector<unitState*>	_reservedState;

	CountryColor::Enum _unitColor;

	image* _lv[4];

	float _moveSpeed;
	int _imageFrameX;

	float _frameTimer;

	//자동 상태인가
	bool _isAuto;
	bool _isMove;

	UnitDirection::DIRECTION _unitDirection;

	unit* _mergeUnit;

	friend class unitCreateMotion;
	friend class unitNoneState;
	friend class unitOneStep;
	friend class unitBuildTown;
	friend class unitRun;
	friend class unitFight;
	friend class unitMerge;
	friend class unitDigObject;

public:
	unit();
	~unit();

	HRESULT init(vector2D index, int height,CountryColor::Enum country);
	void release();
	void update();
	void render();

	void changeState(unitState* newstate);
	void reserveState(unitState* newstate);
	void imageFrame(void);

	vector2D getDirectionVector(UnitDirection::DIRECTION dir);

	CountryColor::Enum getCountryColor() { return _unitColor; }

	bool getIsStarUnit() { return _isStarUnit; }
	void setIsStarUnit(bool isStar) { _isStarUnit = isStar; }

	int getHealth() { return _hp; }
	void setHp(int num) { _hp = num;  if (_hp < 0) _hp = 0; }

	void syncIndexFromPos();
	void requestRender();
	image* setUnitLvImage(int health);
	void setImage(image* lv1, image* lv2, image* lv3);

	string getColorString();
	vector2D getIndex() { return _index; }

	bool isMoveable(POINT index);
	bool isBuildableTown(POINT index);
	unit* isCanRun();
	unit* isCanAttack();
	mncObjectBase* isCanAttackNature();
	unit* isCanCombin();

	void setMergeUnit(unit* mergeUnit);

	void moveAstar(int x, int y);
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
	mncObjectBase* _obj;
	int _frameTimer;
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
class unitDigObject : public unitState
{
private:
	mncObjectBase* _nature;
public:
	unitDigObject() {}
	unitDigObject(mncObjectBase* enemy)
	{
		_nature = enemy;
	}

	virtual void enter(unit& me);
	virtual void update(unit& me);
};
class unitMerge : public unitState
{
private:
public:
	unitMerge(unit* mergeUnit,unit& me)
	{
		me._mergeUnit = mergeUnit;
	}
	virtual void enter(unit& me);
	virtual void update(unit & me);
};