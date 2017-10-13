#pragma once
#include "singletonBase.h"
#include <bitset>
#include <deque>

using namespace std;

#define KEYMAX 256

namespace key
{
	enum Enum
	{
		LEFT, RIGHT, UP, DOWN, ATTACK, MENU, END
	};
}

#define KEYMANAGER keyManager::getSingleton()

class keyManager : public singletonBase<keyManager>
{
private:
	bitset<KEYMAX>	_keyPast;
	bitset<KEYMAX>	_keyCurrent;

	bitset<KEYMAX>	_keyState;

	key::Enum		_keyList[key::END];		//����Ű ����Ʈ

public:
	HRESULT init();
	void release();
	void update();

	bool isOnceKeyDown(int key);
	bool isOnceKeyUp(int key);
	bool isStayKeyDown(int key);

	bool isOnceKeyDown(key::Enum key);
	bool isOnceKeyUp(key::Enum key);
	bool isStayKeyDown(key::Enum key);

	//����Ű ����
	void setupKey(key::Enum key, int setupKey)
	{
		_keyList[key] = (key::Enum)setupKey;
	}

	void setKeyState(int key, bool set) { _keyState.set(key, set); }

	keyManager();
	~keyManager();
};
