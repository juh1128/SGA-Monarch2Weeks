#include "stdafx.h"
#include "keyManager.h"


keyManager::keyManager() {}
keyManager::~keyManager() {}


HRESULT keyManager::init()
{
	//처음엔 모든 키를 눌려있지 않은 상태로 초기화
	_keyCurrent.reset();
	_keyPast.reset();
	_keyState.reset();

	return S_OK;
}
void keyManager::release()
{

}
void keyManager::update()    //gameNode의 update에 추가
{
	//계속 과거키를 현재키로 갱신해준다
	_keyPast = _keyCurrent;
}


bool keyManager::isOnceKeyDown(int key)
{
	//해당 키를 누른 상태
	if (_keyState[key])
	{
		//그 키가 눌려있지 않았다면
		if (!_keyPast[key])
		{
			//현재키를 눌린 상태로 바꾸고 return true
			_keyCurrent.set(key, true);
			return true;
		}
	}
	//해당 키를 누르지 않은 상태
	else _keyCurrent.set(key, false);   //현재키를 누르지 않은 상태로 바꿈

	return false;
}

bool keyManager::isOnceKeyUp(int key)
{
	//해당 키를 누른 상태					현재키를 눌린 상태로 바꿈
	if (_keyState[key]) _keyCurrent.set(key, true);
	//해당 키를 누르지 않은 상태
	else
	{
		//그 키가 눌려있었다면
		if (_keyPast[key])
		{
			//현재키를 누르지 않은 상태로 바꾸고 return true
			_keyCurrent.set(key, false);
			return true;
		}
	}

	return false;
}

bool keyManager::isStayKeyDown(int key)
{
	if (_keyState[key]) return true;

	return false;
}

bool keyManager::isOnceKeyDown(key::Enum key)
{
	return isOnceKeyDown((int)_keyList[key]);
}
bool keyManager::isOnceKeyUp(key::Enum key)		   
{
	return isOnceKeyUp((int)_keyList[key]);
}
bool keyManager::isStayKeyDown(key::Enum key)	   
{
	return isStayKeyDown((int)_keyList[key]);
}
