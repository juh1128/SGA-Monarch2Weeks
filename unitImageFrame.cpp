#include "stdafx.h"
#include "unit.h"

void unit::imageFrame(void)
{
	//float������ ������ ������ �ȵǼ� �Ҽ��� 3�ڸ������� �Ǽ��� ������ _frameTime�� �־���
	int _frameTime = _livedTime * 100;
	//�������� �غ��ϱ� �׷����� ������ ��������
	if (_frameTime % 30 == 0)
	{
		_imageFrameX++;
		if (_imageFrameX > 1)
		{
			_imageFrameX = 0;
		}
	}
}