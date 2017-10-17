#pragma once
#include "gameObject.h"


class bridge : public gameObject
{
private:
	POINT					_index;
	int						_height;

	int						_hp;
	int						_frame;
public:
	bridge() {}
	virtual ~bridge() {}

	HRESULT init();
	void release();
	void update();
	void render();

	int getHp() { return _hp; }
	void addHp(int add) { _hp += add; }
	void setHp(int set) { _hp = set; }
};