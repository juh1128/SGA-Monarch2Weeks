#include "stdafx.h"
#include "unit.h"

void unit::imageFrame(void)
{
	//float형식은 나머지 연산이 안되서 소수점 3자리까지의 실수를 정수형 _frameTime에 넣어줌
	int _frameTime = _livedTime * 100;
	//이정도쯤 해보니까 그럴싸해 보여서 넣은거임
	if (_frameTime % 30 == 0)
	{
		_imageFrameX++;
		if (_imageFrameX > 1)
		{
			_imageFrameX = 0;
		}
	}
}