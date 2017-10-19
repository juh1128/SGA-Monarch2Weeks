#pragma once
#include "mncObjectBase.h"
#include "country.h"


class castleWall : public mncObjectBase
{
private:
	CountryColor::Enum	_countryColor;

	int						_frame;
	bool					_isLeft;

public:
	castleWall() {}
	virtual ~castleWall() {}

	HRESULT init(int xIndex, int yIndex, bool isLeft, CountryColor::Enum color);
	void release();
	void update();
	void render();

	void setDoorWall();
};


//성은 왕+성벽으로 구성됨.
//왕 => 성벽 체력 확인 후 계속 수리한다. 왕의 체력은 100정도? 왕이 죽으면 끝남.
//성벽 => 성 생성 시 성 주변으로 성벽 오브젝트를 생성한다.
class castle : public mncObjectBase
{
private:
	int					_frameX;
	int					_frameY;
	float				_frameTimer;

	castleWall*			_castleWall[8];

public:
	castle() {}
	virtual ~castle() {}

	HRESULT init(int xIndex, int yIndex, CountryColor::Enum color, Direction::Enum direction);
	void release();
	void update();
	void render();

	string getSpriteKey(CountryColor::Enum color);
};
