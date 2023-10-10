#pragma once
#include <GameEngineBase/GameEngineMath.h>

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
		LPCSTR SemanticName,
		DXGI_FORMAT Format,
		D3D11_INPUT_CLASSIFICATION InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
		UINT InstanceDataStepRate = 0,
		UINT AlignedByteOffset = -1,
		UINT InputSlot = 0, 
		UINT SemanticIndex = 0
	);

};

// 설명 :
class GameEngineVertex
{
public:
	static GameEngineInputLayOutInfo LayOut;

public:
	float4 POSITION;
	float4 TEXCOORD;
	float4 COLOR;
	float4 NORMAL;
	float4 BINORMAL;
	float4 TANGENT;
	float4 WEIGHT;
	int BLENDINDICES[4];

};

class GameEngineVertexParticle
{
public:
	static GameEngineInputLayOutInfo LayOut;

public:
	float4 POSITION;
	int PINDEX[4]; // x에 그냥 내가 0번이다 1번이다 2번이다
};