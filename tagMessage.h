#pragma once

class gameObject;

//��� ���ӿ�����Ʈ�� ��⺤�Ϳ� �޽������͸� ������.
//���ӿ�����Ʈ������ �޽����� �ְ� ���� �� �ִ�.
//�޽���ť�� �� �����Ӹ��� �ڵ����� Ŭ����ȴ�.
//health Ŭ�������� ���� ������Ʈ �ȿ��� ���̴� Ŭ��������
//parent->popMessage �Լ��� ���ؼ� �޽��� ó���� �� �� �ִ�.
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

//NULL�޽��� ����
#define NULL_MESSAGE tagMessage("NULL")


//�޼��� ��� ��ũ��
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