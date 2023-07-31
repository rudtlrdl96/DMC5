#pragma once
#include <Windows.h>
#include <string_view>
#include <vector>
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineBase/GameEngineDebug.h>

// 이미지란
// => 컴퓨터에서 뭔가 파일이나 크기를 가지고 있다면
// 특정 저장소의 공간을 차지하고 있는 bit의 집합일수 밖에 없다.
// 그러면 byte의 집합이라고도 할수 있다.
// 모든건.
// 이미지도 저거일수밖에 없고

// 모든 이미지는 2가지만 기억하면 됩니다.
// 1. 포맷 색깔하나를 몇바이트를 써써 표현했는가? 모든 색깔은 RGBA를 통해서 표현합니다.
//    이 RGBA를 1개당 몇바이트를 써서 표현할거냐가 포맷입니다.
//    Bitmap 포맷은 RGBA를 보통 R을 1바이트를 통해서 표현합니다.
// 2. 압축방식.
//    bitmap은 압축하지 않아.

// Bitmap을 사용하면 램에 1062x1380 짜리 int 2차원 
// int ArrBitMap[1380][1062]
// char Color[4]
// Color[0] == 255

// 이걸 윈도우가 관리하기 시작합니다.
// int ArrBitMap[1380][1062] <= 우리는 이걸 보지 못해요.
// 배열에 대한 뭔가를 할수 있는 권한을 주는데 그게 HDC이다.
// HDC는 그래서 색깔의 배열과 연결되어 있고. 그걸 제어할수 있게 도와주는 통로이다.

struct ImageCutData
{
	float StartX = 0.0f;
	float StartY = 0.0f;
	float SizeX = 0.0f;
	float SizeY = 0.0f;

	float4 GetStartPos() 
	{
		return {StartX, StartY};
	}

	float4 GetScale()
	{
		return { SizeX, SizeY };
	}
};

// 설명 :
class GameEnginePath;
class GameEngineImage
{
public:
	// constrcuter destructer
	GameEngineImage();
	~GameEngineImage();

	// delete Function
	GameEngineImage(const GameEngineImage& _Other) = delete;
	GameEngineImage(GameEngineImage&& _Other) noexcept = delete;
	GameEngineImage& operator=(const GameEngineImage& _Other) = delete;
	GameEngineImage& operator=(GameEngineImage&& _Other) noexcept = delete;

	bool ImageCreate(HDC _Hdc);

	bool ImageCreate(const float4& _Scale);

	bool ImageLoad(const GameEnginePath& _Path);

	bool ImageLoad(const std::string_view& _Path);

	void ImageClear();

	HDC GetImageDC() const
	{
		return ImageDC;
	}

	float4 GetImageScale() const
	{
		return float4{ static_cast<float>(Info.bmWidth), static_cast<float>(Info.bmHeight) };
	}

	bool IsImageCutting() 
	{
		return IsCut;
	}

	size_t GetImageCuttingCount()
	{
		return ImageCutDatas.size();
	}

	bool IsCutIndexValid(int _Index) const
	{
		if (0 > _Index)
		{
			return false;
		}

		if (ImageCutDatas.size() <= _Index)
		{
			return false;
		}
		
		return true;
	}

	ImageCutData GetCutData(int _Index) const
	{
		if (false == IsCutIndexValid(_Index))
		{
			MsgAssert("유효하지 않은 컷 인덱스 입니다.");
		}

		return ImageCutDatas[_Index];
	}



	void Cut(float4 _Start, float4 _End, int _X, int _Y);

	void Cut(int _X, int _Y);

	// Copy

	void BitCopy(const GameEngineImage* _OtherImage, float4 _Pos, float4 _Scale);

	// 랜더링을 제외할 컬러.
	// 당연히 느립니다.
	// 크기조절이 느리기 때문에
	void TransCopy(const GameEngineImage* _OtherImage, float4 _CopyCenterPos, float4 _CopySize, float4 _OtherImagePos, float4 _OtherImageSize, int _Color = RGB(255, 0, 255));

	// 디폴트 인자는 선언에서만 가능합니다.
	void TransCopy(const GameEngineImage* _OtherImage, int _CutIndex, float4 _CopyCenterPos, float4 _CopySize, int _Color = RGB(255, 0, 255));

	void AlphaCopy(const GameEngineImage* _OtherImage, float4 _CopyCenterPos, float4 _CopySize, float4 _OtherImagePos, float4 _OtherImageSize, int _Alpha);
	// 디폴트 인자는 선언에서만 가능합니다.
	void AlphaCopy(const GameEngineImage* _OtherImage, int _CutIndex, float4 _CopyCenterPos, float4 _CopySize, int _Color);

	void PlgCopy(const GameEngineImage* _OtherImage, float4 _CopyCenterPos, float4 _CopySize, float4 _OtherImagePos, float4 _OtherImageSize, float _Angle, GameEngineImage* _FilterImage);

	void PlgCopy(const GameEngineImage* _OtherImage, int _CutIndex, float4 _CopyCenterPos, float4 _CopySize, float _Angle, GameEngineImage* _FilterImage);
	// 디폴트 인자는 선언에서만 가능합니다.


	DWORD GetPixelColor(float4 _Pos, DWORD _OutColor);

	DWORD GetPixelColor(int _X, int _Y, DWORD _OutColor);

protected:

private:
	HDC ImageDC = nullptr;
	HBITMAP BitMap = nullptr;
	HBITMAP OldBitMap = nullptr;
	BITMAP Info = BITMAP();
	bool IsCut = false;

	std::vector<ImageCutData> ImageCutDatas;

	void ImageScaleCheck();

};

