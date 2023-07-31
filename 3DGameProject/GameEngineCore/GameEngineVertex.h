#pragma once
#include <GameEngineBase/GameEngineMath.h>



// D3D11_INPUT_ELEMENT_DESC

class GameEngineInputLayOutInfo
{
public:
	static UINT FormatSize(DXGI_FORMAT _Format);

	inline const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetDescs() 
	{
		return Descs;
	}

private:
	std::vector<D3D11_INPUT_ELEMENT_DESC> Descs;
	int Offset = 0;

public:
	void AddInputLayOut(
			LPCSTR SemanticName, // 내가 포지션이다 내가 컬러다.
			DXGI_FORMAT Format, // 내가 n바이트 짜리 정보다 자료형
			D3D11_INPUT_CLASSIFICATION InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA, // 버텍스 데이터가 인스턴싱 데이터다 라는걸 표현
			UINT InstanceDataStepRate = 0, // 나중에 나중에 인스턴싱이라는 구조를 배울때 쓸때가 있을것이다.
			UINT AlignedByteOffset = -1, // 0번째 바이트부터 
			UINT InputSlot = 0, // n개의 버텍스 버퍼를 세팅할때 n번째 버텍스 버퍼의 인풋 레이아웃이다 라는걸 표현하는것
			UINT SemanticIndex = 0
	);

};

// 설명 :
class GameEngineVertex
{
public:
	static GameEngineInputLayOutInfo LayOut;

public:
	float4 POSITION; // 0 16
	float4 UV;    // 16 32
	float4 NORMAL; // 0 16
};

