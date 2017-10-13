#include "stdafx.h"
#include "character.h"


HRESULT character::init(vector2D pos, string imageKey)
{
	gameObject::init("ĳ����", imageKey, pos);

	frame = 0;
	frameTimer = 0.15f;

	_moveSpeed = 4.0f;

	_state = new characterNoneState;
	return S_OK;
}

void character::release()
{
	gameObject::release();
}

void character::update()
{
	gameObject::update();

	frameTimer -= TIMEMANAGER->getElapsedTime();
	if (frameTimer < 0)
	{
		frameTimer += 0.15f;
		frame = ++frame % 4;
	}

	//��ü�� ����ó���� _state��ü���� �����Ѵ�.
	_state->update(*this);
}

void character::render()
{
	gameObject::frameRender(frame, 0);
}

void character::move(int x, int y)
{
	//pathFinder�� ���� ã�� �� ��ȯ�� ���ͷ� moveOneStep ����� ������.
	//deque<terrainTile*> path = PATHFINDER->getPath(WORLD->getTerrainTile(_pos), WORLD->getTerrainTile(vector2D(x, y)));
	//if (path.size() > 0)
	//{
	//	clearReservedState();
	//	changeState(new characterNoneState);
	//}
	//for (size_t i = 0; i < path.size(); ++i)
	//{
	//	reserveState(new characterMoveOneStep(path[i]));
	//}
}

void character::changeState(characterState * newState)
{
	SAFE_DELETE(_state);
	_state = newState;
	_state->enter(*this);
}

void character::clearReservedState()
{
	size_t size = _reservedState.size();
	for (size_t i = 0; i < size; ++i)
	{
		delete _reservedState[0];
		_reservedState.pop_front();
	}
}










//���� ��ü ����
void characterMoveOneStep::enter(character& cCharacter)
{
	cCharacter.changeState(new characterNoneState);
	//if (!_destTile)
	//{
	//	_destTile = WORLD->getTerrainTile(_destPos);
	//}

	//���� ������ Ÿ���� �̵� �Ұ� Ÿ���̸�, �̵��� ��ҵȴ�.
	//if (!_destTile->isWalkable()) cCharacter.changeState(new characterNoneState);

	//������ Ÿ���� �߾� ��ǥ�� ���Ѵ�.
	//_destPos = _destTile->getCenterPos();
}

void characterMoveOneStep::update(character & cCharacter)
{
	////������ Ÿ�ϱ��� �̵��Ѵ�.
	//vector2D distance = _destPos - cCharacter._pos;

	////���� Ÿ���� �̵��ӵ� ���� ���� �޾ƿ´�.
	//_moveRatio = WORLD->getTerrainTile(cCharacter._pos)->_moveRatio;

	//if (distance.getLength() <= cCharacter._moveSpeed*_moveRatio)
	//{
	//	cCharacter._pos = _destPos;
	//	cCharacter.changeState(new characterNoneState);
	//}
	//else
	//{
	//	cCharacter._pos = cCharacter._pos + distance.normalize()*cCharacter._moveSpeed*_moveRatio;
	//}

}

//ĳ���Ͱ� �ƹ��͵� ���ϰ� ���� ��.
void characterNoneState::update(character & cCharacter)
{
	//��� ���͸� Ȯ�� �� ���� ó������ ���� ����� ������, �ش� ����� �����Ѵ�.
	if (cCharacter._reservedState.size() > 0)
	{
		cCharacter.changeState(cCharacter._reservedState[0]);
		cCharacter._reservedState.pop_front();
	}
}
