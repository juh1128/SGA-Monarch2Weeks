#include "stdafx.h"
#include "image.h"

ID2D1HwndRenderTarget* image::_RT = NULL;

//�Ϲ� �̹��� �ʱ�ȭ
HRESULT image::init(ID2D1Bitmap* bitmap, tagLoadedImageInfo loadInfo)
{
	_bitmap = bitmap;
	_loadInfo = loadInfo;

	_size.x = (float)_bitmap->GetPixelSize().width;
	_size.y = (float)_bitmap->GetPixelSize().height;

	_scale.x = 1.0f;
	_scale.y = 1.0f;
	_alpha = 1.0f;

	WICRect rc;
	rc.X = 0;
	rc.Y = 0;
	rc.Width = _size.x;
	rc.Height = _size.y;
	_frameInfo.push_back(rc);

	_maxFrameX = 1;
	_maxFrameY = 1;

	return S_OK;
}

//������ �̹��� �ʱ�ȭ
HRESULT image::init(ID2D1Bitmap* bitmap, tagLoadedImageInfo loadInfo, int maxFrameX, int maxFrameY)
{
	_bitmap = bitmap;
	_loadInfo = loadInfo;

	_size.x = (float)_bitmap->GetPixelSize().width;
	_size.y = (float)_bitmap->GetPixelSize().height;

	_scale.x = 1.0f;
	_scale.y = 1.0f;
	_alpha = 1.0f;

	int frameWidth = _size.x / maxFrameX;
	int frameHeight = _size.y / maxFrameY;

	WICRect rc;
	for (int i = 0; i < maxFrameY; ++i)
	{
		for (int j = 0; j < maxFrameX; ++j)
		{
			rc.X = j * frameWidth;
			rc.Y = i * frameHeight;
			rc.Width = frameWidth;
			rc.Height = frameHeight;
			_frameInfo.push_back(rc);
		}
	}

	_maxFrameX = maxFrameX;
	_maxFrameY = maxFrameY;

	return S_OK;
}

void image::release()
{
	_bitmap->Release();
	_bitmap = NULL;

	if (_pixelCollisionDC)
	{
		DeleteDC(_pixelCollisionDC);
	}
}

//������ �غ�
D2D1::Matrix3x2F image::setupRender(int x, int y)
{
	POINT pt = { x, y };

	//�̵� ��ȯ
	_RT->SetTransform(D2D1::Matrix3x2F::Identity());
	D2D1::Matrix3x2F matTrans = D2D1::Matrix3x2F::Translation(pt.x, pt.y);

	return matTrans;
}


//�̹����� �������Ѵ�.
void image::render(int x, int y, Pivot::Enum pivot, bool isRelativePos)
{
	//scale ����
	_size.x = _size.x*_scale.x;
	_size.y = _size.y*_scale.y;

	//�Ǻ� ����
	vector2D renderPos = getPivotPosition(x, y, pivot);

	//�����ǥ�� �׸���
	if (isRelativePos)
	{
		renderPos = CAMERA->getRelativeVector2D(renderPos);
	}

	//���� ������ ������ ȭ�� ���̸� �׸��� �ʴ´�.
	if (renderPos.x > WINSIZEX || renderPos.x + _size.x < 0 ||
		renderPos.y > WINSIZEY || renderPos.y + _size.y < 0)
	{
		resetRenderOption();
		return;
	}

	D2D1_RECT_F dxArea = ::D2D1::RectF(0.0f, 0.0f, _size.x, _size.y);
	_RT->SetTransform(setupRender(renderPos.x, renderPos.y));
	_RT->DrawBitmap(_bitmap, dxArea, _alpha);

	resetRenderOption();
}

//������ ����
void image::frameRender(int x, int y, int frameX, int frameY, Pivot::Enum pivot, bool isRelativePos)
{
	int frame = frameY*_maxFrameX + frameX;

	//�ش� �������� ������� ����
	_size = this->getFrameSize(frame);
	//scale ����
	_size.x = _size.x*_scale.x;
	_size.y = _size.y*_scale.y;

	//�Ǻ� ����
	vector2D renderPos = getPivotPosition(x, y, pivot);

	//�����ǥ�� �׸���
	if (isRelativePos)
	{
		renderPos = CAMERA->getRelativeVector2D(renderPos);
	}

	//���� ������ ������ ȭ�� ���̸� �׸��� �ʴ´�.
	if (renderPos.x > WINSIZEX || renderPos.x + _size.x < 0 ||
		renderPos.y > WINSIZEY || renderPos.y + _size.y < 0)
	{
		resetRenderOption();
		return;
	}

	//������ �� ũ��
	D2D1_RECT_F dxArea = ::D2D1::RectF(0.0f, 0.0f, _size.x, _size.y);
	D2D1_RECT_F dxSrc = ::D2D1::RectF(_frameInfo[frame].X, _frameInfo[frame].Y,
		_frameInfo[frame].X + _frameInfo[frame].Width, _frameInfo[frame].Y + _frameInfo[frame].Height);

	_RT->SetTransform(setupRender(renderPos.x, renderPos.y));
	_RT->DrawBitmap(_bitmap, dxArea, _alpha, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &dxSrc);

	resetRenderOption();
}

//�ִϸ��̼� ����
void image::aniRender(int x, int y, animation* ani, Pivot::Enum pivot, bool isRelativePos)
{
	int frame = ani->getFrameForSheet();

	//�ش� �������� ������� ����
	_size = this->getFrameSize(frame);
	//scale ����
	_size.x = _size.x*_scale.x;
	_size.y = _size.y*_scale.y;

	//�Ǻ� ����
	vector2D renderPos = getPivotPosition(x, y, pivot);
	//�����ǥ�� �׸���
	if (isRelativePos)
	{
		renderPos = CAMERA->getRelativeVector2D(renderPos);
	}

	//���� ������ ������ ȭ�� ���̸� �׸��� �ʴ´�.
	if (renderPos.x > WINSIZEX || renderPos.x + _size.x < 0 ||
		renderPos.y > WINSIZEY || renderPos.y + _size.y < 0)
	{
		resetRenderOption();
		return;
	}

	//������ �� ũ��
	D2D1_RECT_F dxArea = ::D2D1::RectF(0.0f, 0.0f, _size.x, _size.y);
	D2D1_RECT_F dxSrc = ::D2D1::RectF(_frameInfo[frame].X, _frameInfo[frame].Y,
		_frameInfo[frame].X + _frameInfo[frame].Width, _frameInfo[frame].Y + _frameInfo[frame].Height);

	_RT->SetTransform(setupRender(renderPos.x, renderPos.y));
	_RT->DrawBitmap(_bitmap, dxArea, _alpha, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &dxSrc);

	resetRenderOption();
}

void image::setScaleOption(vector2D scale)
{
	_scale = scale;
}

void image::setAlphaOption(float alpha)
{
	_alpha = alpha;
}

void image::setSizeOption(vector2D size)
{
	_size = size;
}

void image::resetRenderOption()
{
	_alpha = 1.0f;
	_scale.x = 1.0f;
	_scale.y = 1.0f;
	_size.x = _bitmap->GetPixelSize().width;
	_size.y = _bitmap->GetPixelSize().height;
}

void image::setPixelDC(HDC pixelDC)
{
	_pixelCollisionDC = pixelDC;
}

COLORREF image::getPixel(int x, int y)
{
	//�ȼ� �浹�� ��Ʈ�� ������ ������ �׳� ������.
	if (!_pixelCollisionDC) return NULL;
	COLORREF color = GetPixel(_pixelCollisionDC, x, y);
	return color;
}

COLORREF image::getPixel(int imageLeft, int imageTop, int x, int y)
{
	//�ȼ� �浹�� ��Ʈ�� ������ ������ �׳� ������.
	if (!_pixelCollisionDC) return NULL;
	COLORREF color = GetPixel(_pixelCollisionDC, x - imageLeft, y - imageTop);
	return color;
}

vector2D image::getFrameSize(int frame)
{
	return vector2D(_frameInfo[frame].Width, _frameInfo[frame].Height);
}

vector2D image::getPivotPosition(int x, int y, Pivot::Enum pivot)
{
	if (pivot == Pivot::LEFT_TOP) return vector2D(x, y);

	vector2D pivotPos(x, y);

	switch (pivot)
	{
		case Pivot::CENTER:
			pivotPos.x -= _size.x * 0.5f;
			pivotPos.y -= _size.y * 0.5f;
		break;

		case Pivot::BOTTOM:
			pivotPos.x -= _size.x * 0.5f;
			pivotPos.y -= _size.y;
		break;

		case Pivot::TOP:
			pivotPos.x -= _size.x * 0.5f;
		break;
	}

	return pivotPos;
}





