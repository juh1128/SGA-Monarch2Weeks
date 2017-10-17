#pragma once
#include "gameObject.h"

class mncObjectBase : public gameObject
{
protected:
	POINT					_index;
	int						_height;
	int						_hp;
	bool					_walkable;

public:
	mncObjectBase() {}
	virtual ~mncObjectBase() {}

	virtual HRESULT init(string name, string key, int xIndex, int yIndex, int hp, bool walkable);
	virtual void release();
	virtual void update();
	virtual void render();
	void frameRender(int xFrame, int yFrame);


	int getHp() { return _hp; }
	void addHp(int add) { _hp += add; }
	void setHp(int set) { _hp = set; }

};