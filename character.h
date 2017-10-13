#pragma once
#include "pathFinder.h"
#include <deque>

class characterState;

class character : public gameObject
{
private:
	int frame;
	float frameTimer;

	characterState* _state;

	float			_moveSpeed;

	//예약 상태
	deque<characterState*>	_reservedState;


	//상태 객체 패턴 위임
	friend class characterNoneState;
	friend class characterMoveOneStep;

public:
	character() {}
	virtual ~character() {}

	HRESULT init(vector2D pos, string imageKey);
	void release();
	void update();
	void render();

	void move(int x, int y);

	//상태를 예약한다.
	void reserveState(characterState* state)
	{
		_reservedState.push_back(state);
	}
	//상태를 변경한다.
	void changeState(characterState* newState);
	//예약된 상태를 모두 제거한다.
	void clearReservedState();
};


//상태객체 인터페이스
class characterState
{
private:

public:
	virtual ~characterState() {}

	virtual void enter(character& cCharacter) {}
	virtual void update(character& cCharacter) {}
};

//아무것도 안할 때 (상태 없음!)
class characterNoneState : public characterState
{
private:

public:
	virtual void enter(character& cCharacter) {}
	virtual void update(character& cCharacter);

};

//1칸 이동
class characterMoveOneStep : public characterState
{
private:
	terrainTile*	_destTile;
	vector2D		_destPos;
	float			_moveRatio;

public:
	characterMoveOneStep(float x, float y) {
		_destPos.x = x;
		_destPos.y = y;
		_destTile = NULL;
	}
	characterMoveOneStep(terrainTile* tile)
	{
		_destTile = tile;
	}
	virtual void enter(character& cCharacter);
	virtual void update(character& cCharacter);
};