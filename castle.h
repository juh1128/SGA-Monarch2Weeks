#pragma once
#include "gameObject.h"
#include "country.h"


class castleWall : public gameObject
{
private:
	CountryColor::Enum	_countryColor;
	POINT					_index;
	int						_height;

	int						_hp;
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
class castle : public gameObject
{
private:
	CountryColor::Enum	_countryColor;
	POINT				_index;
	int					_height;

	int					_hp;
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

	CountryColor::Enum getCountryColor() { return _countryColor; }
};
