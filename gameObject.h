#pragma once
#include "gameNode.h"
#include <functional>


class gameObject : public gameNode
{
private:
	bool			_isLive;		//���� ������Ʈ�� ����ִ��� ����.
	bool			_isActive;		//���� ������Ʈ Ȱ��ȭ ����.
	float			_destroyDelayTime;

	//�޽��� ó�� ����
	vector<tagMessage> _reservedMessage;
	vector<tagMessage> _messageList;
	unordered_map<string, std::function<void(tagMessage)>> _callbackList;

protected:
	
	Pivot::Enum		_pivot;			//LEFT_TOP: �»���� �������� �׸���. CENTER: pos�� �߾����� �׸���. BOTTOM: pos�� �ϴ����� �׸���.
	image*			_image;			//���� ������Ʈ �̹���
	animation*		_animation;		//���� ������Ʈ�� �ִϸ��̼� ����

	vector2D		_scale;
	vector2D		_size;
	float			_alpha;

public:
	string			_name;
	vector2D		_pos;

	//�̹��� Ű��, ��ġ��
	virtual HRESULT init(string objectName, string imageKey = "", vector2D pos = vector2D(0,0), Pivot::Enum pivot = Pivot::CENTER);

	virtual void release(void);

	//���� ������Ʈ�� �޽��� ó��, ������ Ÿ�� �� ������ ���õ� ó���� ��.
	virtual void update(void);	
	//�̹��� 1�� ¥���ų� �ִϸ��̼��� ����� ��� render�Լ����� �ڵ����� ó������. 
	//(������ �̹��� ó���� frameRender�� ���) 
	virtual void render(void);
	//������ ����
	void frameRender(int frameX, int frameY);

	gameObject() {}
	virtual ~gameObject() {}

	RECT getRect(int frame = 0);

	bool isActiveObject();
	bool isLive() { return _isLive; }
	void setDestroy(float delayTime = 0.0f);
	void setActive(bool set) { _isActive = set; }

	//�޽����� �����Ѵ�.
	void sendMessage(string text, float delayTime = 0.0f, int data = 0, float data2 = 0.0f, POINT ptData = POINT(), vector<gameObject*> targetList = vector<gameObject*>());
	void sendMessage(tagMessage msg);
	//�ݹ� �Լ� ����
	void addCallback(string msgName, std::function<void(tagMessage)> callbackFunc)
	{
		_callbackList.insert(make_pair(msgName, callbackFunc));
	}

	//���� �̹��� ó��
	void setSize(int width, int height) { _size.x = width; _size.y = height; }
	vector2D getSize() { return _size; }
	void setOriginSize();

	void setScale(int scaleX, int scaleY) { _scale.x = scaleX; _scale.y = scaleY; }
	vector2D getScale() { return _scale; }

	void setAlpha(float alpha) { _alpha = alpha; }
	float getAlpha() { return _alpha; }


	//�ִϸ��̼� ����
	//void setAnimation(string animationKeyName);
};