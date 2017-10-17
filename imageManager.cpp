#include "stdafx.h"
#include "imageManager.h"

HRESULT imageManager::init(void)
{
	_hdc = GetDC(_hWnd);

	initializeD2D();

	//DW 초기화
	DWInit();
	addTextFormat(L"맑은고딕", 20);

	return S_OK;
}

void imageManager::release(void)
{
	DeleteDC(_hdc);

	deleteAll();
	safeRelease(_renderTarget);
	safeRelease(_d2dFactory);
	safeRelease(_wicFactory);

	for(int i=0; i<DefaultBrush::end; ++i)
		_dwDefaultBrush[i]->Release();
}

image* imageManager::addImage(string key, wstring directory, bool isUsedPixelCollision)
{
	image* img = findImage(key);

	//만약 이미지가 있으면 그냥 그 이미지를 쓴다
	if (img) return img;

	//1. 파일을 D2D 비트맵 포맷으로 로드한다.
	ID2D1Bitmap* bitmap = createD2DBitmapFromFile(directory);
	if (!bitmap)
	{
		UTIL::createMessageBoxW(directory + L" 이미지 로드 실패");
		return NULL;
	}

	//2. 로드한 비트맵 파일로 이미지 클래스 생성
	image::tagLoadedImageInfo loadInfo = { key, directory };
	img = new image;
	img->init(bitmap, std::move(loadInfo));

	if (isUsedPixelCollision)
	{
		HBITMAP hBit = (HBITMAP)LoadImageW(_hInstance, directory.c_str(), IMAGE_BITMAP, 
			bitmap->GetPixelSize().width, bitmap->GetPixelSize().height, LR_LOADFROMFILE);
		HDC memDC = CreateCompatibleDC(_hdc);

		SelectObject(memDC, hBit);
		DeleteObject(hBit);

		img->setPixelDC(memDC);
	}

	//3. 이미지 리스트에 추가
	_imageList.insert(make_pair(key, img));

	return img;
}

image* imageManager::addFrameImage(string key, wstring directory, int frameX, int frameY, bool isUsedPixelCollision)
{
	image* img = findImage(key);

	//만약 이미지가 있으면 그냥 그 이미지를 쓴다
	if (img) return img;

	//1. 파일을 D2D 비트맵 포맷으로 로드한다.
	ID2D1Bitmap* bitmap = createD2DBitmapFromFile(directory);
	if (!bitmap)
	{
		UTIL::createMessageBoxW(directory + L" 이미지 로드 실패");
		return NULL;
	}

	//2. 로드한 비트맵 파일로 이미지 클래스 생성
	image::tagLoadedImageInfo loadInfo = { key, directory };
	img = new image;
	img->init(bitmap, std::move(loadInfo), frameX, frameY);

	if (isUsedPixelCollision)
	{
		HBITMAP hBit = (HBITMAP)LoadImageW(_hInstance, directory.c_str(), IMAGE_BITMAP,
			bitmap->GetPixelSize().width, bitmap->GetPixelSize().height, LR_LOADFROMFILE);
		HDC memDC = CreateCompatibleDC(_hdc);

		SelectObject(memDC, hBit);
		DeleteObject(hBit);

		img->setPixelDC(memDC);
	}

	//3. 이미지 리스트에 추가
	_imageList.insert(make_pair(key, img));

	return img;
}

void imageManager::deleteImage(string key)
{
	auto iter = _imageList.find(key);
	if (iter != _imageList.end())
	{
		image* img = iter->second;
		img->release();
		delete img;
		img = NULL;
	}
}

image* imageManager::findImage(string key)
{
	auto iter = _imageList.find(key);
	if (iter != _imageList.end())
	{
		return iter->second;
	}
	return NULL;
}

void imageManager::drawText(int x, int y, wstring text, int size, DefaultBrush::Enum defaultBrush, DWRITE_TEXT_ALIGNMENT align, wstring font)
{
	//텍스트 레이아웃 생성
	_dwFactory->CreateTextLayout(
		text.c_str(),
		text.length(),
		_dwTextFormats[font],
		text.length()*size,
		size,
		&_dwLayout
	);

	//레이아웃 셋업
	DWRITE_TEXT_RANGE range;
	range.startPosition = 0;
	range.length = text.length();
	_dwLayout->SetFontSize(size, range);

	_dwLayout->SetTextAlignment(align);

	_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	_renderTarget->DrawTextLayout(D2D1::Point2F(x, y), _dwLayout, _dwDefaultBrush[defaultBrush]);

	_dwLayout->Release();
}

void imageManager::drawText(int x, int y, wstring text, COLORREF rgb, float alpha, int size, DWRITE_TEXT_ALIGNMENT align, wstring font)
{
	//텍스트 레이아웃 생성
	_dwFactory->CreateTextLayout(
		text.c_str(),
		text.length(),
		_dwTextFormats[font],
		text.length()*size,
		size,
		&_dwLayout
	);

	//레이아웃 셋업
	DWRITE_TEXT_RANGE range;
	range.startPosition = 0;
	range.length = text.length();
	_dwLayout->SetFontSize(size, range);
	_dwLayout->SetTextAlignment(align);

	//브러쉬 생성
	ID2D1SolidColorBrush* brush;
	_renderTarget->CreateSolidColorBrush(D2D1::ColorF(rgb, alpha), &brush);

	_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	_renderTarget->DrawTextLayout(D2D1::Point2F(x, y), _dwLayout, brush);

	brush->Release();
	_dwLayout->Release();
}

void imageManager::drawTextField(int x, int y, wstring text, int size, int width, int height, DefaultBrush::Enum defaultBrush, DWRITE_TEXT_ALIGNMENT align, wstring font)
{
	_dwFactory->CreateTextLayout(
		text.c_str(),
		text.length(),
		_dwTextFormats[font],
		width,
		height,
		&_dwLayout
	);

	//레이아웃 셋업
	DWRITE_TEXT_RANGE range;
	range.startPosition = 0;
	range.length = text.length();
	_dwLayout->SetFontSize(size, range);

	_dwLayout->SetTextAlignment(align);

	_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	_renderTarget->DrawTextLayout(D2D1::Point2F(x, y), _dwLayout, _dwDefaultBrush[defaultBrush]);

	_dwLayout->Release();
}

void imageManager::drawTextField(int x, int y, wstring text, COLORREF rgb, int size, int width, int height, float alpha, DWRITE_TEXT_ALIGNMENT align, wstring font)
{
	_dwFactory->CreateTextLayout(
		text.c_str(),
		text.length(),
		_dwTextFormats[font],
		width,
		height,
		&_dwLayout
	);

	//레이아웃 셋업
	DWRITE_TEXT_RANGE range;
	range.startPosition = 0;
	range.length = text.length();
	_dwLayout->SetFontSize(size, range);

	_dwLayout->SetTextAlignment(align);

	//브러쉬 생성
	ID2D1SolidColorBrush* brush;
	_renderTarget->CreateSolidColorBrush(D2D1::ColorF(rgb, alpha), &brush);

	_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	_renderTarget->DrawTextLayout(D2D1::Point2F(x, y), _dwLayout, brush);

	brush->Release();
	_dwLayout->Release();
}

void imageManager::deleteAll()
{
	for (auto iter = _imageList.begin(); iter != _imageList.end(); ++iter)
	{
		iter->second->release();
		delete iter->second;
		iter->second = NULL;
	}
	_imageList.clear();
}

void imageManager::reloadALL()
{
	//디바이스 로스트 상황 발생 시 리소스 모두 해제 후 다시 로드해야 함.
	//1. 현재 로드된 리소스들의 로드 정보를 수집한다.
	vector<image::tagLoadedImageInfo> _loadInfoList;
	auto iter = _imageList.begin();
	for (; iter != _imageList.end(); ++iter)
	{
		_loadInfoList.emplace_back(iter->second->getLoadInfo());
	}

	//2. 리소스 모두 해제
	deleteAll();
	safeRelease(_renderTarget);

	//3. 모든 리소스 다시 로드
	initializeRT();
	for (UINT i = 0; i<_loadInfoList.size(); ++i)
	{
		this->addImage(_loadInfoList[i].key, _loadInfoList[i].directory);
	}
}

void imageManager::initializeD2D(void)
{
	//D2D 팩토리 생성
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_d2dFactory);

	//WIC 팩토리 생성
	CoInitialize(NULL);
	CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&_wicFactory));

	//렌더타겟 초기화
	initializeRT();
}

void imageManager::initializeRT(void)
{
	RECT rc;
	GetClientRect(_hWnd, &rc);

	D2D1_RENDER_TARGET_PROPERTIES dxRTProperties = D2D1::RenderTargetProperties();
	D2D1_SIZE_U dxSize = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

	//렌더타겟 생성
	_d2dFactory->CreateHwndRenderTarget(dxRTProperties,
		D2D1::HwndRenderTargetProperties(_hWnd, dxSize),
		&_renderTarget);

	//이미지 클래스에 RT 연결
	image::setRT(_renderTarget);
}

ID2D1Bitmap * imageManager::createD2DBitmapFromFile(wstring directory)
{
	//1. 디코더를 생성한다.
	IWICBitmapDecoder* ipDecoderPtr = NULL;
	_wicFactory->CreateDecoderFromFilename(directory.c_str(), NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &ipDecoderPtr);

	//2. 디코더에서 프레임을 얻는다.
	IWICBitmapFrameDecode*	ipFramePtr = NULL;
	ipDecoderPtr->GetFrame(0, &ipFramePtr);

	//3. 프레임을 기반으로 포맷 컨버터를 만든다.
	safeRelease(_convertedSrcBmp);
	_wicFactory->CreateFormatConverter(&_convertedSrcBmp);

	_convertedSrcBmp->Initialize(ipFramePtr,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0.0f,
		WICBitmapPaletteTypeCustom);

	//4. 컨버트된 데이터를 기반으로 실제 Direct2D용 비트맵을 만든다.
	ID2D1Bitmap* ipResult = NULL;
	_renderTarget->CreateBitmapFromWicBitmap(_convertedSrcBmp,
		nullptr,
		&ipResult);

	safeRelease(ipDecoderPtr);
	safeRelease(ipFramePtr);

	return ipResult;
}

void imageManager::onResize(UINT width, UINT height)
{
	if (_renderTarget != NULL)
	{
		HRESULT hr = _renderTarget->Resize(D2D1::SizeU(width, height));
		if (hr != S_OK)
		{
			reloadALL();
		}
	}
}



HRESULT imageManager::DWInit()
{
	HRESULT hr;

	_dwFactory = NULL;

	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(*&_dwFactory),
		(IUnknown**)&_dwFactory
	);
	if (FAILED(hr))
		return hr;

	//기본 브러시 생성
	_renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black, 1.0f), &_dwDefaultBrush[DefaultBrush::black]);
	_renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White, 1.0f), &_dwDefaultBrush[DefaultBrush::white]);
	_renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow, 1.0f), &_dwDefaultBrush[DefaultBrush::yello]);
	_renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red, 1.0f), &_dwDefaultBrush[DefaultBrush::red]);
	_renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 1.0f), &_dwDefaultBrush[DefaultBrush::blue]);
	_renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green, 1.0f), &_dwDefaultBrush[DefaultBrush::green]);
	_renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gray, 1.0f), &_dwDefaultBrush[DefaultBrush::gray]);

	return hr;

}
HRESULT imageManager::addTextFormat(wstring fontname, float size)
{
	HRESULT hr;
	IDWriteTextFormat* format = NULL;

	hr = _dwFactory->CreateTextFormat(
		fontname.c_str(),
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		size,
		L"ko",
		&format
	);

	this->_dwTextFormats.insert(make_pair(fontname, format));

	if (FAILED(hr))
		return hr;

	return hr;
}





//============================================================================
// ## 선, 도형 렌더링 ##
//============================================================================

void imageManager::drawLine(D2D1::ColorF::Enum color, float alpha, vector2D start, vector2D end, float strokeWidth)
{
	ID2D1SolidColorBrush* brush;
	_renderTarget->CreateSolidColorBrush(D2D1::ColorF(color,alpha), &brush);

	_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	_renderTarget->DrawLine(D2D1::Point2F(start.x, start.y), D2D1::Point2F(end.x, end.y), brush, strokeWidth);

	brush->Release();
}
void imageManager::drawLine(vector2D start, vector2D end, DefaultBrush::Enum defaultBrush, float strokeWidth)
{
	_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	_renderTarget->DrawLine(D2D1::Point2F(start.x, start.y), D2D1::Point2F(end.x, end.y), _dwDefaultBrush[defaultBrush], strokeWidth);
}

void imageManager::drawRectangle(RECT rc, D2D1::ColorF::Enum color, float alpha, float strokeWidth)
{
	ID2D1SolidColorBrush* brush;
	_renderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);

	_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	_renderTarget->DrawRectangle(D2D1::RectF(rc.left,rc.top,rc.right,rc.bottom), brush, strokeWidth);

	brush->Release();
}
void imageManager::drawRectangle(RECT rc, DefaultBrush::Enum defaultBrush, float strokeWidth)
{
	_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	_renderTarget->DrawRectangle(D2D1::RectF(rc.left, rc.top, rc.right, rc.bottom), _dwDefaultBrush[defaultBrush], strokeWidth);
}

void imageManager::drawEllipse(RECT rc, D2D1::ColorF::Enum color, float alpha, float strokeWidth)
{
	ID2D1SolidColorBrush* brush;
	_renderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	D2D1_ELLIPSE ellipse;
	ellipse.point.x = rc.left + width * 0.5;
	ellipse.point.y = rc.top + height * 0.5;
	ellipse.radiusX = width * 0.5;
	ellipse.radiusY = height * 0.5;

	_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	_renderTarget->DrawEllipse(&ellipse, brush, strokeWidth);

	brush->Release();
}

void imageManager::drawEllipse(RECT rc, DefaultBrush::Enum defaultBrush, float strokeWidth)
{
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	D2D1_ELLIPSE ellipse;
	ellipse.point.x = rc.left + width * 0.5;
	ellipse.point.y = rc.top + height * 0.5;
	ellipse.radiusX = width * 0.5;
	ellipse.radiusY = height * 0.5;

	_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	_renderTarget->DrawEllipse(&ellipse, _dwDefaultBrush[defaultBrush], strokeWidth);
}

void imageManager::fillRectangle(RECT rc, D2D1::ColorF::Enum color, float alpha)
{
	ID2D1SolidColorBrush* brush;
	_renderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);

	_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	_renderTarget->FillRectangle(D2D1::RectF(rc.left, rc.top, rc.right, rc.bottom), brush);

	brush->Release();
}
void imageManager::fillRectangle(RECT rc, DefaultBrush::Enum defaultBrush)
{
	_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	_renderTarget->FillRectangle(D2D1::RectF(rc.left, rc.top, rc.right, rc.bottom), _dwDefaultBrush[defaultBrush]);
}

void imageManager::fillEllipse(RECT rc, D2D1::ColorF::Enum color, float alpha)
{
	ID2D1SolidColorBrush* brush;
	_renderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	D2D1_ELLIPSE ellipse;
	ellipse.point.x = rc.left + width * 0.5;
	ellipse.point.y = rc.top + height * 0.5;
	ellipse.radiusX = width * 0.5;
	ellipse.radiusY = height * 0.5;

	_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	_renderTarget->FillEllipse(&ellipse, brush);

	brush->Release();
}
void imageManager::fillEllipse(RECT rc, DefaultBrush::Enum defaultBrush)
{
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	D2D1_ELLIPSE ellipse;
	ellipse.point.x = rc.left + width * 0.5;
	ellipse.point.y = rc.top + height * 0.5;
	ellipse.radiusX = width * 0.5;
	ellipse.radiusY = height * 0.5;

	_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	_renderTarget->FillEllipse(&ellipse, _dwDefaultBrush[defaultBrush]);
}
