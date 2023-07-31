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
			LPCSTR SemanticName, // ���� �������̴� ���� �÷���.
			DXGI_FORMAT Format, // ���� n����Ʈ ¥�� ������ �ڷ���
			D3D11_INPUT_CLASSIFICATION InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA, // ���ؽ� �����Ͱ� �ν��Ͻ� �����ʹ� ��°� ǥ��
			UINT InstanceDataStepRate = 0, // ���߿� ���߿� �ν��Ͻ��̶�� ������ ��ﶧ ������ �������̴�.
			UINT AlignedByteOffset = -1, // 0��° ����Ʈ���� 
			UINT InputSlot = 0, // n���� ���ؽ� ���۸� �����Ҷ� n��° ���ؽ� ������ ��ǲ ���̾ƿ��̴� ��°� ǥ���ϴ°�
			UINT SemanticIndex = 0
	);

};

// ���� :
class GameEngineVertex
{
public:
	static GameEngineInputLayOutInfo LayOut;

public:
	float4 POSITION; // 0 16
	float4 UV;    // 16 32
	float4 NORMAL; // 0 16
};

