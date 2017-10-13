#pragma once
#include "gameNode.h"
#include <functional>


class gameObject : public gameNode
{
private:
	bool			_isLive;		//게임 오브젝트가 살아있는지 여부.
	bool			_isActive;		//게임 오브젝트 활성화 여부.
	float			_destroyDelayTime;

	//메시지 처리 관련
	vector<tagMessage> _reservedMessage;
	vector<tagMessage> _messageList;
	unordered_map<string, std::function<void(tagMessage)>> _callbackList;

protected:
	
	Pivot::Enum		_pivot;			//LEFT_TOP: 좌상단을 기준으로 그린다. CENTER: pos을 중앙으로 그린다. BOTTOM: pos을 하단으로 그린다.
	image*			_image;			//게임 오브젝트 이미지
	animation*		_animation;		//게임 오브젝트의 애니메이션 정보

	vector2D		_scale;
	vector2D		_size;
	float			_alpha;

public:
	string			_name;
	vector2D		_pos;

	//이미지 키값, 위치값
	virtual HRESULT init(string objectName, string imageKey = "", vector2D pos = vector2D(0,0), Pivot::Enum pivot = Pivot::CENTER);

	virtual void release(void);

	//게임 오브젝트의 메시지 처리, 딜레이 타임 후 삭제와 관련된 처리를 함.
	virtual void update(void);	
	//이미지 1장 짜리거나 애니메이션을 사용할 경우 render함수에서 자동으로 처리해줌. 
	//(프레임 이미지 처리는 frameRender를 사용) 
	virtual void render(void);
	//프레임 렌더
	void frameRender(int frameX, int frameY);

	gameObject() {}
	virtual ~gameObject() {}

	RECT getRect(int frame = 0);

	bool isActiveObject();
	bool isLive() { return _isLive; }
	void setDestroy(float delayTime = 0.0f);
	void setActive(bool set) { _isActive = set; }

	//메시지를 전달한다.
	void sendMessage(string text, float delayTime = 0.0f, int data = 0, float data2 = 0.0f, POINT ptData = POINT(), vector<gameObject*> targetList = vector<gameObject*>());
	void sendMessage(tagMessage msg);
	//콜백 함수 저장
	void addCallback(string msgName, std::function<void(tagMessage)> callbackFunc)
	{
		_callbackList.insert(make_pair(msgName, callbackFunc));
	}

	//각종 이미지 처리
	void setSize(int width, int height) { _size.x = width; _size.y = height; }
	vector2D getSize() { return _size; }
	void setOriginSize();

	void setScale(int scaleX, int scaleY) { _scale.x = scaleX; _scale.y = scaleY; }
	vector2D getScale() { return _scale; }

	void setAlpha(float alpha) { _alpha = alpha; }
	float getAlpha() { return _alpha; }


	//애니메이션 셋팅
	//void setAnimation(string animationKeyName);
};