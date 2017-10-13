#include "stdafx.h"
#include "keyManager.h"


keyManager::keyManager() {}
keyManager::~keyManager() {}


HRESULT keyManager::init()
{
	//ó���� ��� Ű�� �������� ���� ���·� �ʱ�ȭ
	_keyCurrent.reset();
	_keyPast.reset();
	_keyState.reset();

	return S_OK;
}
void keyManager::release()
{

}
void keyManager::update()    //gameNode�� update�� �߰�
{
	//��� ����Ű�� ����Ű�� �������ش�
	_keyPast = _keyCurrent;
}


bool keyManager::isOnceKeyDown(int key)
{
	//�ش� Ű�� ���� ����
	if (_keyState[key])
	{
		//�� Ű�� �������� �ʾҴٸ�
		if (!_keyPast[key])
		{
			//����Ű�� ���� ���·� �ٲٰ� return true
			_keyCurrent.set(key, true);
			return true;
		}
	}
	//�ش� Ű�� ������ ���� ����
	else _keyCurrent.set(key, false);   //����Ű�� ������ ���� ���·� �ٲ�

	return false;
}

bool keyManager::isOnceKeyUp(int key)
{
	//�ش� Ű�� ���� ����					����Ű�� ���� ���·� �ٲ�
	if (_keyState[key]) _keyCurrent.set(key, true);
	//�ش� Ű�� ������ ���� ����
	else
	{
		//�� Ű�� �����־��ٸ�
		if (_keyPast[key])
		{
			//����Ű�� ������ ���� ���·� �ٲٰ� return true
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
