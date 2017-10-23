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
		Stop, Search, CreateMotion, Fight, Destroy, Run, BuildTown, NoMoney, Merge, End
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
	vector<terrainTile*> _myTiles;

	//유닛 명령 내리기
	float		 _commandTime;
	terrainTile* _commandDestTile;
	unit*		 _commandTargetUnit;
	string		 _commandStateName;

	friend class unitCreateMotion;
	friend class unitNoneState;
	friend class unitOneStep;
	friend class unitBuildTown;
	friend class unitBuildObject;
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

	int getHeight() { return _height; }
	void setHeight(int set) { _height = set; }

	bool isMoveable(POINT index);
	bool isBuildableTown(POINT index);
	unit* isCanRun();
	unit* isCanAttack();
	mncObjectBase* isCanAttackNature();

	unit* isCanMerge(unit* mergeunit);

	void setAuto(bool autounit) { _isAuto = autounit; }
	
	void setUnitState(UnitState::Enum state) { _state = state; }
	void setCommand(terrainTile* destTile, unit* targetUnit, string stateName)
	{
		_commandTime = TIMEMANAGER->getWorldTime();
		_commandDestTile = destTile;
		_commandTargetUnit = targetUnit;
		_commandStateName = stateName;
	}
	void resetCommand()
	{
		_commandTime = 0;
		_commandDestTile = NULL;
		_commandTargetUnit = NULL;
		_commandStateName = "";
	}
	//명령을 받은 시간을 반환한다. (명령이 없으면 false)
	//명령이 원군일 경우 그냥 0.5가 반환된다.
	float getCommandTime()
	{
		if (_commandStateName == "원군") return 0.5f;
		else if (_commandStateName == "대기") return 0.75f;
		return _commandTime;
	}
	string getCommandName() { return _commandStateName; }
	unit* getCommandTarget() { return _commandTargetUnit; }

};

class unitState
{
private:
	
public:
	string _stateName;
	virtual ~unitState() {}

	virtual void enter(unit& unit) {}
	virtual void update(unit& unit) {}
};

class unitNoneState : public unitState
{
private:
	
public:
	unitNoneState() { _stateName = "NONE"; }
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

	bool _isFinish;

public:
	unitOneStep(int xIndex, int yIndex)
	{
		_stateName = "한걸음";
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
	unitBuildTown(POINT index)
	{ 
		_stateName = "건설";
		_destIndex = index;
	}
	virtual void enter(unit& me);
	virtual void update(unit& me);
};

class unitBuildObject : public unitState
{
private:
	terrainTile* _destTile;
	string _key;
	mncObjectBase* _obj;
	int _frameTimer;
public:
	unitBuildObject(terrainTile* tile, string objectKey)
	{
		_destTile = tile;
		_key = objectKey;
		_stateName = "건설";
	}
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
	unitCreateMotion() 
	{
		_stateName = "탄생";
	}
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
		_stateName = "도망";
		_avoidUnit = avoidUnit;
	}
	virtual void enter(unit& me);
	virtual void update(unit& me) {}
};

class unitFight : public unitState
{
private:
	unit* _enemyUnit;
	int _frameTimer;

public:
	unitFight() {}
	unitFight(unit* enemy)
	{
		_stateName = "전투";
		_enemyUnit = enemy;
		_frameTimer = 0;
	}

	virtual void enter(unit& me);
	virtual void update(unit& me);
};

class unitDigObject : public unitState
{
private:
	mncObjectBase* _nature;
	int _frameTimer;

public:
	unitDigObject() {}
	unitDigObject(mncObjectBase* enemy)
	{
		_stateName = "파괴";
		_nature = enemy;
		_frameTimer = 0;
	}

	virtual void enter(unit& me);
	virtual void update(unit& me);
};

class unitMerge : public unitState
{
private:
	vector2D _oldMergeUnitIndex;
public:
	unitMerge(unit* mergeUnit,unit& me)
	{
		_stateName = "머지";
		me._mergeUnit = mergeUnit;

	}
	virtual void enter(unit& me);
	virtual void update(unit & me);
};