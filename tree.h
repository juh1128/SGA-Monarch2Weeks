#pragma once
#include "gameObject.h"


class tree : public gameObject
{
private:
	POINT			_index;
	int				_height;
	int				_frame;

	int				_hp;

public:
	tree() {}
	virtual ~tree() {}

	HRESULT init(int xIndex, int yIndex);
	void release();
	void update();
	void render();

	int getHp() { return _hp; }
	void addHp(int add) { _hp += add; }
	void setHp(int set) { _hp = set; }
};

class weed : public gameObject
{
private:
	POINT			_index;
	int				_height;

	int				_hp;

public:
	weed() {}
	virtual ~weed() {}

	HRESULT init(int xIndex, int yIndex);
	void release();
	void update();
	void render();

	int getHp() { return _hp; }
	void addHp(int add) { _hp += add; }
	void setHp(int set) { _hp = set; }
};