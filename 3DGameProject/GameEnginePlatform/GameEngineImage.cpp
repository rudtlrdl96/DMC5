#include "PrecompileHeader.h"
#include "GameEngineImage.h"
#include <GameEngineBase/GameEnginePath.h>
#include <GameEngineBase/GameEngineDebug.h>
#include "GameEngineWindow.h"

// 다른 lib를 사용하겠다.
#pragma comment(lib, "msimg32.lib")

GameEngineImage::GameEngineImage()
{
}

GameEngineImage::~GameEngineImage()
{
	if (nullptr != BitMap)
	{
		DeleteObject(BitMap);
		BitMap = nullptr;
	}

	if (nullptr != OldBitMap)
	{
		DeleteObject(OldBitMap);
		OldBitMap = nullptr;
	}

	if (nullptr != ImageDC)
	{
		DeleteDC(ImageDC);
		ImageDC = nullptr;
	}
}

bool GameEngineImage::ImageCreate(HDC _Hdc)
{
	if (nullptr == _Hdc)
	{
		MsgAssert("이미지 생성에 실패했습니다 nullptr 인 HDC를 넣어줬습니다.");
		return false;
	}

	ImageDC = _Hdc;
	ImageScaleCheck();
	return true;
}

void GameEngineImage::ImageClear()
{
	Rectangle(ImageDC, 0, 0, Info.bmWidth, Info.bmHeight);
}

bool GameEngineImage::ImageCreate(const float4& _Scale)
{
	if (true == _Scale.IsZero())
	{
		MsgAssert("크기가 0인 이미지를 만들 수는 없습니다");
		return false;
	}

	BitMap = CreateCompatibleBitmap(GameEngineWindow::GetWindowBackBufferHdc(), _Scale.ix(), _Scale.iy());

	if (nullptr == BitMap)
	{
		MsgAssert("이미지 생성에 실패했습니다.");
		return false;
	}

	ImageDC = CreateCompatibleDC(nullptr);

	if (nullptr == ImageDC)
	{
		MsgAssert("이미지 HDC 생성에 실패했습니다.");
		return false;
	}

	// ImageDC 1,1 배열이랑 연결되어 있다. 

	// 1, 1
	OldBitMap = static_cast<HBITMAP>(SelectObject(ImageDC, BitMap));

	ImageScaleCheck();

	ImageClear();

	return true;
}

bool GameEngineImage::ImageLoad(const GameEnginePath& _Path)
{
	return ImageLoad(_Path.GetFullPath().c_str());
}

#define TEST(Value) Value

bool GameEngineImage::ImageLoad(const std::string_view& _Path)
{
	//HDC ImageDC;
	//HBITMAP BitMap;
	//HBITMAP OldBitMap;
	//BITMAP Info;

	// 이미지중에 일부만 로드할수 있는데 0을 넣어주면 다 로드하겠다는 이야기가 도힙니다.
	// LR_LOADFROMFILE 파일에서부터 로드하겠다는 의미가 됩니다.

	// 이미지를 로드한 2차원 배열의 정보고
	// 윈도우에게 new를 지시한것과 다름이 없다.
	BitMap = static_cast<HBITMAP>(LoadImageA(nullptr, _Path.data(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));

	if (nullptr == BitMap)
	{
		std::string Path = _Path.data();
		MsgAssert(Path + " 이미지 로드에 실패했습니다.");
		return false;
	}

	ImageDC = CreateCompatibleDC(nullptr);

	if (nullptr == ImageDC)
	{
		std::string Path = _Path.data();
		MsgAssert(Path + " 이미지 HDC 생성에 실패했습니다.");
		return false;
	}

	// ImageDC 1,1 배열이랑 연결되어 있다. 

	// 1, 1
	OldBitMap = static_cast<HBITMAP>(SelectObject(ImageDC, BitMap));

	ImageScaleCheck();

	return true;
}

void GameEngineImage::ImageScaleCheck()
{
	HBITMAP CurrentBitMap = static_cast<HBITMAP>(GetCurrentObject(ImageDC, OBJ_BITMAP));
	GetObject(CurrentBitMap, sizeof(BITMAP), &Info);
}


// Copy
void GameEngineImage::BitCopy(const GameEngineImage* _OtherImage, float4 _CenterPos, float4 _Scale)
{
	BitBlt(
		ImageDC, // 복사 당할 이미지
		_CenterPos.ix() - _Scale.hix(), // 위치 
		_CenterPos.iy() - _Scale.hiy(),
		_Scale.ix(),
		_Scale.iy(),
		_OtherImage->GetImageDC(), // 복사할 이미지
		0,
		0,
		SRCCOPY
	);
}

// 구현쪽에서는 디폴트 인자를 표시할 필요가 없습니다.
void GameEngineImage::TransCopy(const GameEngineImage* _OtherImage, int _CutIndex, float4 _CopyCenterPos, float4 _CopySize, int _Color/* = RGB(255, 0, 255)*/)
{
	if (false == _OtherImage->IsCut)
	{
		MsgAssert(" 잘리지 않은 이미지로 cut출력 함수를 사용하려고 했습니다.");
		return;
	}

	ImageCutData Data = _OtherImage->GetCutData(_CutIndex);

	TransCopy(_OtherImage, _CopyCenterPos, _CopySize, Data.GetStartPos(), Data.GetScale(), _Color);
}

void GameEngineImage::TransCopy(const GameEngineImage* _OtherImage, float4 _CopyCenterPos, float4 _CopySize, float4 _OtherImagePos, float4 _OtherImageSize, int _Color)
{

	TransparentBlt(ImageDC, // 여기에 그려라.
		_CopyCenterPos.ix() - _CopySize.hix(), // 여기를 시작으로
		_CopyCenterPos.iy() - _CopySize.hiy(),
		_CopySize.ix(), // 이 크기로
		_CopySize.iy(),
		_OtherImage->GetImageDC(),
		_OtherImagePos.ix(),// 이미지의 x y에서부터
		_OtherImagePos.iy(),
		_OtherImageSize.ix(), // 이미지의 x y까지의 위치를
		_OtherImageSize.iy(),
		_Color);
}


void GameEngineImage::AlphaCopy(const GameEngineImage* _OtherImage, int _CutIndex, float4 _CopyCenterPos, float4 _CopySize, int _Color)
{
	if (false == _OtherImage->IsCut)
	{
		MsgAssert(" 잘리지 않은 이미지로 cut출력 함수를 사용하려고 했습니다.");
		return;
	}

	ImageCutData Data = _OtherImage->GetCutData(_CutIndex);

	AlphaCopy(_OtherImage, _CopyCenterPos, _CopySize, Data.GetStartPos(), Data.GetScale(), _Color);
}

void GameEngineImage::AlphaCopy(const GameEngineImage* _OtherImage, float4 _CopyCenterPos, float4 _CopySize, float4 _OtherImagePos, float4 _OtherImageSize, int _Alpha)
{
	BLENDFUNCTION BF;

	BF.BlendOp = AC_SRC_OVER;
	BF.BlendFlags = 0;
	BF.SourceConstantAlpha = _Alpha;
	BF.AlphaFormat = AC_SRC_ALPHA;

	AlphaBlend(ImageDC, // 여기에 그려라.
		_CopyCenterPos.ix() - _CopySize.hix(), // 여기를 시작으로
		_CopyCenterPos.iy() - _CopySize.hiy(),
		_CopySize.ix(), // 이 크기로
		_CopySize.iy(),
		_OtherImage->GetImageDC(),
		_OtherImagePos.ix(),// 이미지의 x y에서부터
		_OtherImagePos.iy(),
		_OtherImageSize.ix(), // 이미지의 x y까지의 위치를
		_OtherImageSize.iy(),
		BF);
}

void GameEngineImage::PlgCopy(const GameEngineImage* _OtherImage, int _CutIndex, float4 _CopyCenterPos, float4 _CopySize, float _Angle, GameEngineImage* _FilterImage)
{
	if (false == _OtherImage->IsCut)
	{
		MsgAssert(" 잘리지 않은 이미지로 cut출력 함수를 사용하려고 했습니다.");
		return;
	}

	ImageCutData Data = _OtherImage->GetCutData(_CutIndex);

	PlgCopy(_OtherImage, _CopyCenterPos, _CopySize, Data.GetStartPos(), Data.GetScale(), _Angle, _FilterImage);
}

void GameEngineImage::PlgCopy(const GameEngineImage* _OtherImage, float4 _CopyCenterPos, float4 _CopySize, float4 _OtherImagePos, float4 _OtherImageSize, float _Angle, GameEngineImage* _FilterImage)
{
	POINT ArrRotPoint[3];

	CollisionDataAPI Data = { float4::ZERO, _CopySize };

	float4 LeftTop = Data.LeftTop();
	float4 RightTop = Data.RightTop();
	float4 LeftBot = Data.LeftBot();

	ArrRotPoint[0] = (LeftTop.RotationZDegReturn(_Angle) + _CopyCenterPos).ToWindowPOINT();
	ArrRotPoint[1] = (RightTop.RotationZDegReturn(_Angle) + _CopyCenterPos).ToWindowPOINT();
	ArrRotPoint[2] = (LeftBot.RotationZDegReturn(_Angle) + _CopyCenterPos).ToWindowPOINT();

	PlgBlt(ImageDC, // 여기에 그려라.
		ArrRotPoint,
		_OtherImage->GetImageDC(),
		_OtherImagePos.ix(),// 이미지의 x y에서부터
		_OtherImagePos.iy(),
		_OtherImageSize.ix(), // 이미지의 x y까지의 위치를
		_OtherImageSize.iy(),
		_FilterImage->BitMap,
		_OtherImagePos.ix(),
		_OtherImagePos.iy()
	);

}
//
//void GameEngineImage::PlgCopy(const GameEngineImage* _OtherImage, int _CutIndex, float4 _CopyCenterPos, float4 _CopySize, float _Angle, GameEngineImage* _FilterImage)
//{
//
//}

void GameEngineImage::Cut(int _X, int _Y)
{
	ImageCutData Data;

	Data.SizeX = static_cast<float>(GetImageScale().ix() / _X);
	Data.SizeY = static_cast<float>(GetImageScale().iy() / _Y);

	for (size_t i = 0; i < _Y; i++)
	{
		for (size_t i = 0; i < _X; i++)
		{
			ImageCutDatas.push_back(Data);
			Data.StartX += Data.SizeX;
		}

		Data.StartX = 0.0f;
		Data.StartY += Data.SizeY;
	}

	IsCut = true;
}

void GameEngineImage::Cut(float4 _Start, float4 _End, int _X, int _Y)
{
	ImageCutData Data;

	Data.SizeX = static_cast<float>((_End.x - _Start.x) / _X);
	Data.SizeY = static_cast<float>((_End.y - _Start.y) / _Y);

	Data.StartX = _Start.x;
	Data.StartY = _Start.y;

	for (size_t i = 0; i < _Y; i++)
	{
		for (size_t i = 0; i < _X; i++)
		{
			ImageCutDatas.push_back(Data);
			Data.StartX += Data.SizeX;
		}

		Data.StartX = _Start.x;
		Data.StartY += Data.SizeY;
	}

	IsCut = true;
}

DWORD GameEngineImage::GetPixelColor(float4 _Pos, DWORD _OutColor)
{
	return GetPixelColor(_Pos.ix(), _Pos.iy(), _OutColor);
}

DWORD GameEngineImage::GetPixelColor(int _X, int _Y, DWORD _OutColor)
{
	if (_X < 0)
	{
		return _OutColor;
	}
	if (_X >= GetImageScale().ix())
	{
		return _OutColor;
	}
	if (_Y < 0)
	{
		return _OutColor;
	}
	if (_Y >= GetImageScale().iy())
	{
		return _OutColor;
	}

	return GetPixel(ImageDC, _X, _Y);
}