#pragma once
#include <vector>

namespace Pivot
{
	enum Enum
	{
		LEFT_TOP, CENTER, BOTTOM, TOP
	};
}

class image
{
public:
	struct tagLoadedImageInfo
	{
		string key;
		wstring directory;
	};
	
private:
	static ID2D1HwndRenderTarget* _RT;

	ID2D1Bitmap*		_bitmap;
	float				_alpha;			//�̹��� ����

	vector2D			_size;			//�̹��� ũ��
	vector2D			_scale;			//x,y ũ�� Ȯ��,��� ����

	tagLoadedImageInfo	_loadInfo;		//�̹��� �ε� ����

	//�ȼ� �浹�� DC
	HDC	_pixelCollisionDC;

	//������ ���� ó���� ���� RECT���� ����
	vector<WICRect>		_frameInfo;
	int					_maxFrameX;
	int					_maxFrameY;

public:
	image() { 
		_bitmap = NULL;
		_pixelCollisionDC = NULL;
	}
	virtual ~image() {}

	HRESULT init(ID2D1Bitmap* bitmap, tagLoadedImageInfo loadInfo);
	HRESULT init(ID2D1Bitmap* bitmap, tagLoadedImageInfo loadInfo, int maxFrameX, int maxFrameY);
	void release();

	//�̹��� ������
	void render(int x, int y, Pivot::Enum pivot = Pivot::CENTER, bool isRelativePos = true);
	void frameRender(int x, int y, int frameX, int frameY, Pivot::Enum pivot = Pivot::CENTER, bool isRelativePos = true);
	void aniRender(int x, int y, animation* ani, Pivot::Enum pivot = Pivot::CENTER, bool isRelativePos = true);

	void setScaleOption(vector2D scale);
	void setAlphaOption(float alpha);
	//������ �̹����� setSize�� ������ų �� ����. scale�� ����ϼ���.
	void setSizeOption(vector2D size);

	void resetRenderOption();

	//==================================================
	UINT getWidth() { return _bitmap->GetPixelSize().width; }
	UINT getHeight() { return _bitmap->GetPixelSize().height; }
	vector2D getFrameSize(int frame);

	tagLoadedImageInfo getLoadInfo() { return _loadInfo; }

	void setPixelDC(HDC pixelDC);

	//==================================================

	//�ȼ� �浹�� �Լ�
	COLORREF getPixel(int imageLeft, int imageTop, int x, int y);
	COLORREF getPixel(int x, int y);
	HDC getPixelDC() { return _pixelCollisionDC; }

public:
	static void setRT(ID2D1HwndRenderTarget* rt) 
	{ 
		_RT = rt; 
	}

private:
	D2D1::Matrix3x2F setupRender(int x, int y);
	vector2D getPivotPosition(int x,int y,Pivot::Enum pivot);
};

