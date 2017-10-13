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

	//���� ����
	deque<characterState*>	_reservedState;


	//���� ��ü ���� ����
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

	//���¸� �����Ѵ�.
	void reserveState(characterState* state)
	{
		_reservedState.push_back(state);
	}
	//���¸� �����Ѵ�.
	void changeState(characterState* newState);
	//����� ���¸� ��� �����Ѵ�.
	void clearReservedState();
};


//���°�ü �������̽�
class characterState
{
private:

public:
	virtual ~characterState() {}

	virtual void enter(character& cCharacter) {}
	virtual void update(character& cCharacter) {}
};

//�ƹ��͵� ���� �� (���� ����!)
class characterNoneState : public characterState
{
private:

public:
	virtual void enter(character& cCharacter) {}
	virtual void update(character& cCharacter);

};

//1ĭ �̵�
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