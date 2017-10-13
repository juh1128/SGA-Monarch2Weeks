#pragma once
#include "singletonBase.h"
#include <unordered_map>

//���̷�Ʈ2D ���̺귯��
#pragma comment(lib, "d2d1.lib")
#include <d2d1.h>
#include <d2d1helper.h>

//���̷�Ʈ Write ���̺귯�� (�ؽ�Ʈ)
#pragma comment(lib, "dwrite.lib")
#include <dwrite.h>

//WIC ���̺귯��
#pragma comment(lib, "WindowsCodecs.lib")
#include <wincodec.h>

#define IMAGEMANAGER imageManager::getSingleton()
#define RT imageManager::getSingleton()->getRenderTarget()

#include "image.h"

namespace DefaultBrush
{
	enum Enum
	{
		white, black, yello, red, blue, green, gray, end
	};
}


class imageManager : public singletonBase<imageManager>
{
private:
	unordered_map<string, image*>		_imageList;

	//���̷�Ʈ2D (������ ����)
	ID2D1Factory*						_d2dFactory;
	ID2D1HwndRenderTarget*				_renderTarget;

	//WIC (�̹��� ó�� ������Ʈ)
	IWICImagingFactory*					_wicFactory;
	IWICFormatConverter*				_convertedSrcBmp;

	//���̷�Ʈ Write (�ؽ�Ʈ ����)
	IDWriteFactory*								_dwFactory;
	unordered_map<wstring, IDWriteTextFormat*>	_dwTextFormats;
	IDWriteTextLayout*							_dwLayout;
	ID2D1SolidColorBrush*						_dwDefaultBrush[DefaultBrush::end];

	//�ȼ� �浹�� HDC
	HDC _hdc;

public:

	//�̹��� �Ŵ��� �ʱ�ȭ (���� �ý��� ���� �ʱ�ȭ)
	HRESULT init();
	//�̹��� �Ŵ��� ����
	void release();

	//�̹��� �߰�
	image* addImage(string key, wstring directory, bool isUsedPixelCollision = false);	
	//������ �̹��� �߰�
	image* addFrameImage(string key, wstring directory, int frameX, int frameY, bool isUsedPixelCollision = false);
	void deleteImage(string key);
	image* findImage(string key);

	//�ؽ�Ʈ ��� (�⺻ �귯�� ���)
	void drawText(int x, int y, wstring text, int size, DefaultBrush::Enum defaultBrush = DefaultBrush::black, DWRITE_TEXT_ALIGNMENT align = DWRITE_TEXT_ALIGNMENT_CENTER, wstring font = L"�������");
	//�ؽ�Ʈ ��� (���ϴ� ����� ���İ����� �귯�� �����ؼ� ���)
	void drawText(int x,int y,wstring text, COLORREF rgb, float alpha, int size, DWRITE_TEXT_ALIGNMENT align = DWRITE_TEXT_ALIGNMENT_CENTER, wstring font = L"�������");
	//�ؽ�Ʈ �ʵ� ��� (�⺻ �귯�� ���)
	void drawTextField(int x, int y, wstring text, int size, int width, int height, DefaultBrush::Enum defaultBrush = DefaultBrush::black, DWRITE_TEXT_ALIGNMENT align = DWRITE_TEXT_ALIGNMENT_CENTER, wstring font = L"�������");
	//�ؽ�Ʈ �ʵ� ��� (���ϴ� ����� ���İ����� �귯�� �����ؼ� ���)
	void drawTextField(int x, int y, wstring text, COLORREF rgb, int size, int width, int height, float alpha = 1.0f, DWRITE_TEXT_ALIGNMENT align = DWRITE_TEXT_ALIGNMENT_CENTER, wstring font = L"�������");

	//==============================================================
	// ## ���� �׸��� �Լ� ##

	void drawLine(D2D1::ColorF::Enum color, float alpha, vector2D start, vector2D end, float strokeWidth = 1);
	void drawLine(vector2D start, vector2D end, DefaultBrush::Enum defaultBrush = DefaultBrush::black, float strokeWidth = 1);
	void drawRectangle(RECT rc, D2D1::ColorF::Enum color, float alpha, float strokeWidth = 1);
	void drawRectangle(RECT rc, DefaultBrush::Enum defaultBrush = DefaultBrush::black, float strokeWidth = 1);
	void drawEllipse(RECT rc, D2D1::ColorF::Enum color, float alpha, float strokeWidth = 1);
	void drawEllipse(RECT rc, DefaultBrush::Enum defaultBrush = DefaultBrush::black, float strokeWidth = 1);
	void fillRectangle(RECT rc, D2D1::ColorF::Enum color, float alpha);
	void fillRectangle(RECT rc, DefaultBrush::Enum defaultBrush = DefaultBrush::black);
	void fillEllipse(RECT rc, D2D1::ColorF::Enum color, float alpha);
	void fillEllipse(RECT rc, DefaultBrush::Enum defaultBrush = DefaultBrush::black);
	//==============================================================



	void deleteAll();
	void reloadALL();

	ID2D1HwndRenderTarget*	getRenderTarget() { return _renderTarget; }

	void onResize(UINT width, UINT height);

	imageManager() 
	{
		_d2dFactory = NULL;
		_renderTarget = NULL;
		_wicFactory = NULL;
		_convertedSrcBmp = NULL;
	}
	~imageManager() {}

private:

	void initializeD2D(void);
	void initializeRT(void);
	ID2D1Bitmap* createD2DBitmapFromFile(wstring directory);

	//���̷�Ʈ Write
	HRESULT DWInit();
	//��Ʈ �߰�
	HRESULT addTextFormat(wstring fontname, float size);

};