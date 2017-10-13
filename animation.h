#pragma once
#include <vector>

class animation
{
public:
	struct tagAnimationInfo
	{
		int frame;			//������ �� ��Ʈ�� frame
		float delayTime;
		string messageName;
		int offsetY;
	};
private:
	int _nowFrame;
	int _maxFrame;
	bool _loop;
	vector<tagAnimationInfo>	_frameList;

public:
	animation() {}
	virtual ~animation() {}

	void init();
	void release();
	void update();

	void setDefPlayFrame(bool loop = false);
	void setPlayFrame(int* playArr, int arrLen, bool loop = false);
	void setPlayFrame(int start, int end, bool loop = false);

	void start();
	void stop();
	void pause();
	void resume();

	//��Ʈ���� �� ��° ������������ �ƴ϶� ���� �ִϸ��̼ǿ��� ���° ���������� ��ȯ
	int getFrame() { return _nowFrame; } 
	//��Ʈ���� �� ��° ����������?
	int getFrameForSheet() { return _frameList[_nowFrame].frame; }

	void addFrame();
	void setFrame(int set);

	void setFrameOption(int frame, string message, int yOffset) 
	{ 
		_frameList[frame].messageName = message; 
		_frameList[frame].offsetY = yOffset;
	}
};