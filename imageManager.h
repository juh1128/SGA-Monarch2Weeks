#pragma once
#include "singletonBase.h"
#include <unordered_map>

//다이렉트2D 라이브러리
#pragma comment(lib, "d2d1.lib")
#include <d2d1.h>
#include <d2d1helper.h>

//다이렉트 Write 라이브러리 (텍스트)
#pragma comment(lib, "dwrite.lib")
#include <dwrite.h>

//WIC 라이브러리
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

	//다이렉트2D (렌더링 엔진)
	ID2D1Factory*						_d2dFactory;
	ID2D1HwndRenderTarget*				_renderTarget;

	//WIC (이미지 처리 컴포넌트)
	IWICImagingFactory*					_wicFactory;
	IWICFormatConverter*				_convertedSrcBmp;

	//다이렉트 Write (텍스트 엔진)
	IDWriteFactory*								_dwFactory;
	unordered_map<wstring, IDWriteTextFormat*>	_dwTextFormats;
	IDWriteTextLayout*							_dwLayout;
	ID2D1SolidColorBrush*						_dwDefaultBrush[DefaultBrush::end];

	//픽셀 충돌용 HDC
	HDC _hdc;

public:

	//이미지 매니저 초기화 (각종 시스템 변수 초기화)
	HRESULT init();
	//이미지 매니저 해제
	void release();

	//이미지 추가
	image* addImage(string key, wstring directory, bool isUsedPixelCollision = false);	
	//프레임 이미지 추가
	image* addFrameImage(string key, wstring directory, int frameX, int frameY, bool isUsedPixelCollision = false);
	void deleteImage(string key);
	image* findImage(string key);

	//텍스트 출력 (기본 브러쉬 사용)
	void drawText(int x, int y, wstring text, int size, DefaultBrush::Enum defaultBrush = DefaultBrush::black, DWRITE_TEXT_ALIGNMENT align = DWRITE_TEXT_ALIGNMENT_CENTER, wstring font = L"맑은고딕");
	//텍스트 출력 (원하는 색상과 알파값으로 브러쉬 생성해서 출력)
	void drawText(int x,int y,wstring text, COLORREF rgb, float alpha, int size, DWRITE_TEXT_ALIGNMENT align = DWRITE_TEXT_ALIGNMENT_CENTER, wstring font = L"맑은고딕");
	//텍스트 필드 출력 (기본 브러쉬 사용)
	void drawTextField(int x, int y, wstring text, int size, int width, int height, DefaultBrush::Enum defaultBrush = DefaultBrush::black, DWRITE_TEXT_ALIGNMENT align = DWRITE_TEXT_ALIGNMENT_CENTER, wstring font = L"맑은고딕");
	//텍스트 필드 출력 (원하는 색상과 알파값으로 브러쉬 생성해서 출력)
	void drawTextField(int x, int y, wstring text, COLORREF rgb, int size, int width, int height, float alpha = 1.0f, DWRITE_TEXT_ALIGNMENT align = DWRITE_TEXT_ALIGNMENT_CENTER, wstring font = L"맑은고딕");

	//==============================================================
	// ## 도형 그리기 함수 ##

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

	//다이렉트 Write
	HRESULT DWInit();
	//폰트 추가
	HRESULT addTextFormat(wstring fontname, float size);

};