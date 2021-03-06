#include "stdafx.h"
#include "castle.h"

HRESULT castle::init(int xIndex, int yIndex, CountryColor::Enum color, Direction::Enum direction)
{
	mncObjectBase::init("군주", getSpriteKey(color), xIndex, yIndex, 100, false);

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
	//성벽 삭제
	for (int i = 0; i < 8; ++i)
	{
		if (!_castleWall[i]) continue;

		if (_castleWall[i]->isLive())
		{
			_castleWall[i]->release();
			delete _castleWall[i];
			_castleWall[i] = NULL;
		}
	}

	//해당 국가 게임오버
	country* cty = WORLD->getCountry(_countryColor);
	if (cty)
	{
		if(cty->isLive())
		cty->gameOver();
	}

	mncObjectBase::release();

	terrainTile* tile = WORLD->getMap()->getTile(_index.x, _index.y);
	tile->setBuildable(false);
}

void castle::update()
{
	mncObjectBase::update();

	//왕 프레임 업데이트
	_frameTimer += TIMEMANAGER->getElapsedTime();
	if (_frameTimer >= 0.5f)
	{
		_frameTimer -= 0.5f;
		_frameX = ++_frameX % 2;
	}

	//성문 업데이트
	for (int i = 0; i < 8; ++i)
	{
		if (_castleWall[i])
		{
			if (_castleWall[i]->isLive())
			{
				_castleWall[i]->update();
			}
			else
			{
				_castleWall[i]->release();
				delete _castleWall[i];
				_castleWall[i]->setDestroy();
				_castleWall[i] = NULL;
			}
		}
	}
}

void castle::render()
{
	float zoom = CAMERA->getZoom();
	float imageHalfHeight = _image->getFrameSize(_frameX).y * zoom * 0.5f;

	_image->setAlphaOption(_alpha);
	_image->setScaleOption(vector2D(zoom, zoom));
	_image->frameRender(_pos.x*zoom, _pos.y*zoom + imageHalfHeight - 10, _frameX, _frameY, _pivot);
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
			str = "whiteKing";
		break;
		case CountryColor::BLUE:
			str = "blueKing";
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
	mncObjectBase::init("성문", "castle", xIndex, yIndex, 500, false);

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
	mncObjectBase::release();

	terrainTile* tile = WORLD->getMap()->getTile(_index.x, _index.y);
	tile->setBuildable(false);
}

void castleWall::update()
{
	mncObjectBase::update();

	//성문 자동수리
	if (_hp < _maxHp)
	{
		//국고에서 돈을 가져옴.
		if (WORLD->getCountry(_countryColor)->getGold() >= 10)
		{
			WORLD->getCountry(_countryColor)->addGold(-10);
			_hp += 10;
		}
	}
}

void castleWall::render()
{
	mncObjectBase::frameRender(_frame, 0);
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
