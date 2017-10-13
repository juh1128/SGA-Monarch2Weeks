#include "stdafx.h"
#include "character.h"


HRESULT character::init(vector2D pos, string imageKey)
{
	gameObject::init("캐릭터", imageKey, pos);

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

	//객체의 상태처리를 _state객체에게 위임한다.
	_state->update(*this);
}

void character::render()
{
	gameObject::frameRender(frame, 0);
}

void character::move(int x, int y)
{
	//pathFinder로 길을 찾은 뒤 반환된 벡터로 moveOneStep 명령을 내린다.
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










//상태 객체 패턴
void characterMoveOneStep::enter(character& cCharacter)
{
	cCharacter.changeState(new characterNoneState);
	//if (!_destTile)
	//{
	//	_destTile = WORLD->getTerrainTile(_destPos);
	//}

	//만약 목적지 타일이 이동 불가 타일이면, 이동이 취소된다.
	//if (!_destTile->isWalkable()) cCharacter.changeState(new characterNoneState);

	//목적지 타일의 중앙 좌표를 구한다.
	//_destPos = _destTile->getCenterPos();
}

void characterMoveOneStep::update(character & cCharacter)
{
	////목적지 타일까지 이동한다.
	//vector2D distance = _destPos - cCharacter._pos;

	////현재 타일의 이동속도 배율 값을 받아온다.
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

//캐릭터가 아무것도 안하고 있을 때.
void characterNoneState::update(character & cCharacter)
{
	//명령 벡터를 확인 후 아직 처리하지 않은 명령이 있으면, 해당 명령을 수행한다.
	if (cCharacter._reservedState.size() > 0)
	{
		cCharacter.changeState(cCharacter._reservedState[0]);
		cCharacter._reservedState.pop_front();
	}
}
