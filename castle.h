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


//���� ��+�������� ������.
//�� => ���� ü�� Ȯ�� �� ��� �����Ѵ�. ���� ü���� 100����? ���� ������ ����.
//���� => �� ���� �� �� �ֺ����� ���� ������Ʈ�� �����Ѵ�.
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
