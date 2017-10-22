#include "stdafx.h"



HRESULT gameObject::init(string objectName, string imageKey, vector2D pos, Pivot::Enum pivot)
{
	_name = objectName;

	_animation = NULL;
	//이미지가 있을 경우
	if (imageKey != "")
	{
		_image = IMAGEMANAGER->findImage(imageKey);
		_size = _image->getFrameSize(0);
		//_animation->init(_image->getWidth(), _image->getHeight(), _image->getFrameWidth(), _image->getFrameHeight());
	}
	//이미지가 없을 경우
	else
	{
		_image = NULL;
		_size = vector2D(1, 1);
		//_animation->init(1, 1, 1, 1);
	}

	_pos = pos;
	_pivot = pivot;

	_isLive = true;
	_isActive = true;
	_destroyDelayTime = 0;

	_scale = vector2D(1, 1);
	_alpha = 1.0f;

	return S_OK;
}

RECT gameObject::getRect(int frame)
{
	RECT rc = {};

	if (_image)
	{
		vector2D scaledSize = (frame == 0)?_size * _scale : _image->getFrameSize(frame) * _scale;
		//설정된 피봇에 따라 렉트를 만든다.
		switch (_pivot)
		{
			case Pivot::LEFT_TOP:
			{
				rc = RectMake(_pos.x, _pos.y, scaledSize.x, scaledSize.y);
			}
			break;

			case Pivot::TOP:
				rc = RectMake(_pos.x - (scaledSize.x * 0.5f), _pos.y, scaledSize.x, scaledSize.y);
			break;

			case Pivot::CENTER:
			{
				rc = RectMakeCenter(_pos.x, _pos.y, scaledSize.x, scaledSize.y);
			}
			break;

			case Pivot::BOTTOM:
			{
				rc.left = _pos.x - scaledSize.x *0.5f;
				rc.right = _pos.x + scaledSize.x *0.5f;
				rc.top = _pos.y - scaledSize.y;
				rc.bottom = _pos.y;
			}
			break;
		}
	}


	return rc;
}

void gameObject::release(void)
{
	_reservedMessage.clear();
	_messageList.clear();

	SAFE_RELEASE(_animation);
	SAFE_DELETE(_animation);
}

void gameObject::update(void)
{
	//게임오브젝트 삭제 딜레이 타임이 활성화 됬을 경우
	if (_destroyDelayTime > 0.0f)
	{
		_destroyDelayTime -= TIMEMANAGER->getElapsedTime();
		if (_destroyDelayTime <= 0.0f)
		{
			_isLive = false;
		}
	}

	//메시지 예약 리스트 업데이트
	float elapsedTime = TIMEMANAGER->getElapsedTime();
	for (int i = 0; i < _reservedMessage.size(); ++i)
	{
		_reservedMessage[i].delayTime -= elapsedTime;
		//딜레이 타임이 다 됬으면 메시지 리스트로 옮긴다.
		if (_reservedMessage[i].delayTime <= 0.0f)
		{
			_reservedMessage[i].delayTime = 0.0f;
			_messageList.emplace_back(_reservedMessage[i]);
			_reservedMessage.erase(_reservedMessage.begin() + i);
			--i;
		}
	}

	//메시지 처리하기
	for (int i = _messageList.size() - 1; i >= 0; --i)
	{
		auto iter = _callbackList.find(_messageList[i].text);
		if (iter != _callbackList.end())
		{
			iter->second.callbackFunc(_messageList[i]);

			//해당 메시지 관찰자들에게 응답 메시지 전달
			auto& observerList = iter->second.observerList;
			size_t size = observerList.size();
			vector<gameObject*> me;
			me.push_back(this);

			for (size_t i = 0; i < size; ++i)
			{
				observerList[i]->sendMessage("observed_" + _messageList[i].text, 0, 0, 0, POINT(), me);
			}
		}
		_messageList.erase(_messageList.begin()+i);
	}
}

void gameObject::render(void)
{
	if (_image)
	{
		_image->setAlphaOption(_alpha);
		_image->setScaleOption(_scale);
		_image->setSizeOption(_size);
		if (_animation)
		{
			_image->aniRender(_pos.x, _pos.y, _animation, _pivot);
		}
		else
		{
			_image->render(_pos.x, _pos.y, _pivot);
		}
	}
}

void gameObject::frameRender(int frameX, int frameY)
{
	//카메라와 상대좌표를 구해서 카메라에 비춰지지 않고 있으면 렌더링되지 않는다.
	if (_image)
	{
		_image->setAlphaOption(_alpha);
		_image->setScaleOption(_scale);
		_image->frameRender(_pos.x, _pos.y, frameX, frameY, _pivot);
	}
}

bool gameObject::isActiveObject()
{
	//오브젝트가 죽었다면 무조건 false
	if (!_isLive)
		return false;

	//오브젝트는 살아있지만 active가 false일 경우
	if (!_isActive)
		return false;

	return true;
}

void gameObject::setDestroy(float delayTime)
{
	if (delayTime <= 0.0f)
	{
		_isLive = false;
	}
	else
	{
		_destroyDelayTime = delayTime;
	}
}

//메시지 처리 관련
void gameObject::sendMessage(string text, float delayTime, int data, float data2, POINT ptData, vector<gameObject*> targetList)
{
	//딜레이타임이 없으면 바로 메시지 벡터에 넣는다.
	tagMessage msg(text, delayTime, data, data2, ptData, targetList);
	if (delayTime == 0.0f)
	{
		_messageList.push_back(msg);
	}
	//딜레이타임이 있으면 메시지 예약 리스트에 넣는다.
	else
	{
		_reservedMessage.push_back(msg);
	}
}

void gameObject::sendMessage(tagMessage msg)
{
	if (msg.delayTime == 0.0f)
	{
		_messageList.emplace_back(msg);
	}
	else
	{
		_reservedMessage.emplace_back(msg);
	}
}

void gameObject::addCallback(string msgName, std::function<void(tagMessage)> callbackFunc)
{
	auto& cbInfo = _callbackList[msgName];
	cbInfo.callbackFunc = callbackFunc;
}

void gameObject::addObserver(string observeMessage, gameObject * observer)
{	
	auto& cbInfo = _callbackList[observeMessage];
	//중복 체크
	for (size_t i = 0; i < cbInfo.observerList.size(); ++i)
	{
		if (cbInfo.observerList[i] == observer)
		{
			return;
		}
	}
	cbInfo.observerList.push_back(observer);
}

void gameObject::removeObserver(string observeMessage, gameObject * observer)
{
	auto& cbInfo = _callbackList[observeMessage];
	//콜백 정보가 비어있다면 아무것도 안함.
	if (cbInfo.observerList.size() == 0 && cbInfo.callbackFunc == NULL)
	{
		return;
	}

	size_t size = cbInfo.observerList.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (cbInfo.observerList[i] == observer)
		{
			cbInfo.observerList.erase(cbInfo.observerList.begin() + i);
			break;
		}
	}
}

vector<gameObject*> gameObject::getObserverList(string observeMessage)
{
	return _callbackList[observeMessage].observerList;
}

void gameObject::setOriginSize()
{
	if (_image)
	{
		_size.x = _image->getWidth();
		_size.y = _image->getHeight();
	}
	else
	{
		_size = vector2D(1, 1);
	}
}


//void gameObject::setAnimation(string animationKeyName)
//{
//	RECT oldRC = this->getCollisionRect();
//
//	auto anim = KEYANIMANAGER->findAnimation(animationKeyName);
//	*_animation = *anim;
//	_animation->start();
//
//	_size.x = _animation->getFrameWidth();
//	_size.y = _animation->getFrameHeight();
//
//	RECT newRC = this->getCollisionRect();
//
//	//애니메이션 사이즈에 따라서 y위치 보정
//	float gapY = oldRC.bottom - newRC.bottom;
//	_pos.y += gapY;
//}