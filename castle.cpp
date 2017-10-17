#include "stdafx.h"
#include "castle.h"

HRESULT castle::init(int xIndex, int yIndex, CountryColor::Enum color, Direction::Enum direction)
{
	terrainTile* tile = WORLD->getMap()->getTile(xIndex, yIndex);
	_height = tile->getHeight();

	//해당 x,y,height 위치의 타일에게 성을 등록함.
	tile->setObjectOnTile(this);

	gameObject::init("캐슬", getSpriteKey(color), tileMap::getTilePosFromIndex(vector2D(xIndex, yIndex), _height));
	_pos = _pos / CAMERA->getZoom();

	_index.x = xIndex;
	_index.y = yIndex;
	_countryColor = color;
	_frameX = 0;
	_frameTimer = 0;

	//성벽 생성
	int isLeft = false;
	if (direction == Direction::Left || direction == Direction::Right)
	{
		isLeft = true;
	}

	auto createWall = [this](castleWall** wall, int xIndex, int yIndex, bool isLeft, CountryColor::Enum color)
	{
		*wall = new castleWall;
		(*wall)->init(xIndex, yIndex, isLeft, this->getCountryColor());
	};

	createWall(&_castleWall[0], xIndex-1, yIndex-1, isLeft, _countryColor);
	createWall(&_castleWall[1], xIndex+0, yIndex-1,  isLeft, _countryColor);
	createWall(&_castleWall[2], xIndex+1, yIndex -1, isLeft, _countryColor);
	createWall(&_castleWall[3], xIndex -1, yIndex+0, isLeft, _countryColor);
	createWall(&_castleWall[4], xIndex+1, yIndex+0,  isLeft, _countryColor);
	createWall(&_castleWall[5], xIndex -1, yIndex+1, isLeft, _countryColor);
	createWall(&_castleWall[6], xIndex+0, yIndex+1,  isLeft, _countryColor);
	createWall(&_castleWall[7], xIndex+1, yIndex+1,  isLeft, _countryColor);

	//성문 설정
	switch (direction)
	{
		case Direction::Right:
			_castleWall[1]->setDoorWall();
			_frameY = 1;
		break;
		case Direction::Left:
			_castleWall[6]->setDoorWall();
			_frameY = 3;
		break;
		case Direction::Up:
			_castleWall[3]->setDoorWall();
			_frameY = 0;
		break;
		case Direction::Down:
			_castleWall[4]->setDoorWall();
			_frameY = 2;
		break;
	}

	return S_OK;
}

void castle::release()
{
	terrainTile* tile = WORLD->getMap()->getTile(_index.x, _index.y);
	tile->removeObjectOnTile();

	//성벽 삭제
	for (int i = 0; i < 8; ++i)
	{
		if (_castleWall[i]->isLive())
		{
			_castleWall[i]->release();
			delete _castleWall[i];
			_castleWall[i] = NULL;
		}
	}

	gameObject::release();
}

void castle::update()
{
	gameObject::update();

	//왕 프레임 업데이트
	_frameTimer += TIMEMANAGER->getElapsedTime();
	if (_frameTimer >= 0.5f)
	{
		_frameTimer -= 0.5f;
		_frameX = ++_frameX % 2;
	}
}

void castle::render()
{
	if (_image)
	{
		float zoom = CAMERA->getZoom();
		_image->setAlphaOption(_alpha);
		_image->setScaleOption(vector2D(zoom, zoom));
		_image->frameRender(_pos.x*zoom, _pos.y*zoom, _frameX, _frameY, _pivot);
	}
}

string castle::getSpriteKey(CountryColor::Enum color)
{
	string str;
	switch (color)
	{
		case CountryColor::RED:
			str = "redKing";
		break;
		case CountryColor::WHITE:
			str = "redKing";
		break;
		case CountryColor::BLUE:
			str = "greenKing";
		break;
		case CountryColor::GREEN:
			str = "greenKing";
		break;
	}
	return str;
}






//================================================================
// - 성문
HRESULT castleWall::init(int xIndex, int yIndex, bool isLeft, CountryColor::Enum color)
{
	terrainTile* tile = WORLD->getMap()->getTile(xIndex, yIndex);
	_height = tile->getHeight();

	tile->setObjectOnTile(this);

	gameObject::init("성문", "castle", tileMap::getTilePosFromIndex(vector2D(xIndex, yIndex), _height));
	_pos = _pos / CAMERA->getZoom();

	_index.x = xIndex;
	_index.y = yIndex;
	_countryColor = color;

	//프레임 셋팅
	if(isLeft)
	{ 
		_frame = 1;
	}
	else
	{
		_frame = 3;
	}
	_isLeft = isLeft;

	return S_OK;
}

void castleWall::release()
{
	terrainTile* tile = WORLD->getMap()->getTile(_index.x, _index.y);
	tile->removeObjectOnTile();

	gameObject::release();
}

void castleWall::update()
{
	gameObject::update();
}

void castleWall::render()
{
	if (_image)
	{
		float zoom = CAMERA->getZoom();
		_image->setAlphaOption(_alpha);
		_image->setScaleOption(vector2D(zoom, zoom));
		_image->frameRender(_pos.x*zoom, _pos.y*zoom, _frame, 0, _pivot);
	}
}

void castleWall::setDoorWall()
{
	if (_isLeft)
	{
		_frame = 0;
	}
	else
	{
		_frame = 2;
	}
}
