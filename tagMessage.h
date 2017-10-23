#pragma once

class gameObject;

//모든 게임오브젝트는 대기벡터와 메시지벡터를 가진다.
//게임오브젝트끼리는 메시지를 주고 받을 수 있다.
//메시지큐는 매 프레임마다 자동으로 클리어된다.
//health 클래스같이 게임 오브젝트 안에서 쓰이는 클래스들은
//parent->popMessage 함수를 통해서 메시지 처리를 할 수 있다.
struct tagMessage
{
	bool isActive;
	string text;
	int data;
	float data2;
	POINT ptData;
	vector<gameObject*> targetList;

	float delayTime;

	tagMessage();
	tagMessage(string text, float delay = 0.0f, int data = 0, float data2 = 0.0f, POINT ptData = POINT(), vector<gameObject*> targetList = vector<gameObject*>());
};

//NULL메시지 정의
#define NULL_MESSAGE tagMessage("NULL")


//메세지 통신 매크로
#define USING_MESSAGE_CALLBACK \
private:\
	vector<tagMessage> _reservedMessage;\
	vector<tagMessage> _messageList;\
	unordered_map<string, std::function<void(tagMessage)>> _callbackList; \
public: \
	void sendMessage(string text, float delayTime = 0.0f, int data = 0, float data2 = 0.0f, POINT ptData = POINT(), vector<gameObject*> targetList = vector<gameObject*>());\
	void addCallback(string msgName, std::function<void(tagMessage)> callbackFunc)\
	{\
		_callbackList.insert(make_pair(msgName, callbackFunc));\
	}\
private: 

#define RESERVED_MESSAGE_PROCESS \
float elapsedTime = TIMEMANAGER->getElapsedTime();\
for (int i = 0; i < _reservedMessage.size(); ++i)\
{\
	_reservedMessage[i].delayTime -= elapsedTime;\
	if (_reservedMessage[i].delayTime <= 0.0f)\
	{\
		_reservedMessage[i].delayTime = 0.0f;\
		_messageList.emplace_back(_reservedMessage[i]);\
		_reservedMessage.erase(_reservedMessage.begin() + i);\
		--i;\
	}\
}\

#define MESSAGE_PROC \
for (int i = _messageList.size() - 1; i >= 0; --i)\
{\
	auto iter = _callbackList.find(_messageList[i].text);\
	if (iter != _callbackList.end())\
	{\
		iter->second(_messageList[i]);\
	}\
	_messageList.erase(_messageList.begin() + i);\
}