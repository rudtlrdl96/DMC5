#include "PrecompileHeader.h"
#include "GameEngineMaterial.h"
#include <GameEngineCore/GameEngineDevice.h>
#include "GameEngineVertexBuffer.h"
#include "GameEngineIndexBuffer.h"
#include "GameEngineVertexShader.h"
#include "GameEngineRasterizer.h"
#include "GameEnginePixelShader.h"
#include "GameEngineBlend.h"
#include "GameEngineInputLayOut.h"
#include "GameEngineDepthState.h"

GameEngineMaterial::GameEngineMaterial()
{
	// InputLayOutPtr = std::make_shared<GameEngineInputLayOut>();
}

GameEngineMaterial::~GameEngineMaterial()
{
}

// �Ž� + ��Ƽ����
//
//// ���� ���� ������ �غ��ϰ�
//void GameEngineMaterial::InputAssembler1() 
//{
//	if (nullptr == InputLayOutPtr)
//	{
//		MsgAssert("��ǲ ���̾ƿ��� �������� �ʾƼ� ��ǲ�����1 ������ ������ �� �����ϴ�.");
//		return;
//	}
//
//	// �������� ���ؼ��� ������ ���� ����°� �⺻�̴�.
//	// �׷���ī��� ���� ��� ������ ������ �ִ��� ���� ���Ѵ�.
//	InputLayOutPtr->Setting();
//
//	if (nullptr == VertexBufferPtr)
//	{
//		MsgAssert("���ؽ� ���۰� �������� �ʾƼ� ��ǲ�����1 ������ ������ �� �����ϴ�.");
//		return;
//	}
//
//	// D3D11 ERROR: ID3D11DeviceContext::DrawIndexed: The Vertex Shader expects 
//	// application provided input data (which is to say data other than hardware 
//	// auto-generated values such as VertexID or InstanceID). Therefore an Input 
//	// Assembler object is expected, but none is bound. 
//	// [ EXECUTION ERROR #349: DEVICE_DRAW_INPUTLAYOUT_NOT_SET]
//
//	VertexBufferPtr->Setting();
//	// GameEngineDevice::GetContext()->IASetVertexBuffers()
//}
// ���ÿ� �����ϴ� ���� �츮�� ���� ����� ���ؼ� ��ȯ�ϰ�.
void GameEngineMaterial::VertexShader()
{
	if (nullptr == VertexShaderPtr)
	{
		MsgAssert("���ؽ� ���̴��� �������� �ʾƼ� ���ؽ� ���̴� ������ ������ �� �����ϴ�.");
		return;
	}

	VertexShaderPtr->Setting();
}
//
//// ���� ������ ���� � ������ �׸��� ���ϰ�
//void GameEngineMaterial::InputAssembler2() 
//{
//	// �׸��� ������ ���� �����͸� �־��ش�.
//	if (nullptr == IndexBufferPtr)
//	{
//		MsgAssert("�ε��� ���۰� �������� �ʾƼ� ��ǲ �����2 ������ ������ �� �����ϴ�.");
//		return;
//	}
//
//	// 012023
//	IndexBufferPtr->Setting();
//
//	// 012 023 <=3���� ��� ������ ������� ���⼭ ó���� �Ǿ���.
//	GameEngineDevice::GetContext()->IASetPrimitiveTopology(TOPOLOGY);
//}

// ���⼭����
void GameEngineMaterial::HullShader()
{

}
void GameEngineMaterial::Tessellator()
{

}
void GameEngineMaterial::DomainShader()
{

}
void GameEngineMaterial::GeometryShaeder()
{

}
// ��������� ȭ����� ��� ���� ����

// w�����⸦ ���ݴϴ�. 
// ����Ʈ�� �����ݴϴ�.
// ȭ�� �ø� 
// �ȼ� ������
void GameEngineMaterial::Rasterizer()
{
	if (nullptr == RasterizerPtr)
	{
		MsgAssert("�����Ͷ������� �������� �ʾƼ� ������ �Ұ����մϴ�.");
		return;
	}

	RasterizerPtr->SetFILL_MODE(FILL_MODE);
	RasterizerPtr->Setting();

	// GameEngineDevice::GetContext()->RSSetState
}


void GameEngineMaterial::PixelShader()
{
	if (nullptr == PixelShaderPtr)
	{
		MsgAssert("�ȼ� ���̴��� �������� �ʾƼ� �ȼ� ���̴� ������ ������ �� �����ϴ�.");
		return;
	}

	PixelShaderPtr->Setting();


	// GameEngineDevice::GetContext()->PSSetShader
}
void GameEngineMaterial::OutputMerger()
{
	if (nullptr == BlendStatePtr)
	{
		MsgAssert("���尡 �������� �ʾ� �ƿ�ǲ ���� ������ �Ϸ��Ҽ��� �����ϴ�.");
		return;
	}


	BlendStatePtr->Setting();

	if (nullptr == DepthStatePtr)
	{
		MsgAssert("���尡 �������� �ʾ� �ƿ�ǲ ���� ������ �Ϸ��Ҽ��� �����ϴ�.");
		return;
	}
	DepthStatePtr->Setting();
}

//
//void GameEngineMaterial::SetVertexBuffer(const std::string_view& _Value)
//{
//	std::string UpperName = GameEngineString::ToUpper(_Value);
//	VertexBufferPtr = GameEngineVertexBuffer::Find(UpperName);
//
//	if (nullptr == VertexBufferPtr)
//	{
//		MsgAssert("�������� �ʴ� ���ؽ� ���۸� ����Ϸ��� �߽��ϴ�.");
//	}
//
//	if (nullptr == VertexShaderPtr)
//	{
//		return;
//	}
//
//	InputLayOutPtr->ResCreate(VertexBufferPtr, VertexShaderPtr);
//}
//
//
//void GameEngineMaterial::SetIndexBuffer(const std::string_view& _Value)
//{
//	std::string UpperName = GameEngineString::ToUpper(_Value);
//	IndexBufferPtr = GameEngineIndexBuffer::Find(UpperName);
//
//	if (nullptr == IndexBufferPtr)
//	{
//		MsgAssert("�������� �ʴ� ���ؽ� ���۸� ����Ϸ��� �߽��ϴ�.");
//	}
//}



void GameEngineMaterial::SetVertexShader(const std::string_view& _Value)
{
	std::string UpperName = GameEngineString::ToUpper(_Value);
	VertexShaderPtr = GameEngineVertexShader::Find(UpperName);

	if (nullptr == VertexShaderPtr)
	{
		MsgAssert("�������� �ʴ� ���ؽ� ���̴��� ����Ϸ��� �߽��ϴ�.");
	}

	if (nullptr == VertexBufferPtr)
	{
		return;
	}

	// InputLayOutPtr->ResCreate(VertexBufferPtr, VertexShaderPtr);
}


void GameEngineMaterial::SetPixelShader(const std::string_view& _Value)
{
	std::string UpperName = GameEngineString::ToUpper(_Value);
	PixelShaderPtr = GameEnginePixelShader::Find(UpperName);

	if (nullptr == PixelShaderPtr)
	{
		MsgAssert("�������� �ʴ� �ȼ� ���̴��� ����Ϸ��� �߽��ϴ�.");
	}
}

void GameEngineMaterial::SetBlendState(const std::string_view& _Value)
{
	std::string UpperName = GameEngineString::ToUpper(_Value);
	BlendStatePtr = GameEngineBlend::Find(UpperName);

	if (nullptr == BlendStatePtr)
	{
		MsgAssert("�������� �ʴ� ���带 �����Ϸ��� �߽��ϴ�.");
		return;
	}
}

void GameEngineMaterial::SetDepthState(const std::string_view& _Value)
{
	std::string UpperName = GameEngineString::ToUpper(_Value);
	DepthStatePtr = GameEngineDepthState::Find(UpperName);

	if (nullptr == DepthStatePtr)
	{
		MsgAssert("�������� �ʴ� ���̹��� ������Ʈ�� �����Ϸ��� �߽��ϴ�.");
		return;
	}
}

void GameEngineMaterial::SetRasterizer(const std::string_view& _Value)
{
	std::string UpperName = GameEngineString::ToUpper(_Value);
	RasterizerPtr = GameEngineRasterizer::Find(UpperName);

	if (nullptr == RasterizerPtr)
	{
		MsgAssert("�������� �ʴ� �����Ͷ������� ����Ϸ��� �߽��ϴ�.");
	}
}

void GameEngineMaterial::RenderingPipeLineSetting()
{
	// ������� �ϴ� �κ��� ������ ������������ �ѹٲ� ������ ��.
	// InputAssembler1();
	VertexShader();
	// InputAssembler2();
	HullShader();
	Tessellator();
	DomainShader();
	GeometryShaeder();
	Rasterizer();
	PixelShader();
	OutputMerger();
}

void GameEngineMaterial::Render()
{
	//UINT IndexCount = IndexBufferPtr->GetIndexCount();
	//GameEngineDevice::GetContext()->DrawIndexed(IndexCount, 0, 0);
}

std::shared_ptr<GameEngineMaterial> GameEngineMaterial::Clone()
{
	std::shared_ptr<GameEngineMaterial> ClonePipe = std::make_shared<GameEngineMaterial>();

	// ClonePipe->InputLayOutPtr = InputLayOutPtr;
	ClonePipe->VertexBufferPtr = VertexBufferPtr;
	// ClonePipe->IndexBufferPtr = IndexBufferPtr;
	ClonePipe->VertexShaderPtr = VertexShaderPtr;
	ClonePipe->RasterizerPtr = RasterizerPtr;
	ClonePipe->PixelShaderPtr = PixelShaderPtr;
	ClonePipe->BlendStatePtr = BlendStatePtr;
	ClonePipe->DepthStatePtr = DepthStatePtr;
	ClonePipe->IsCloneValue = true;

	return ClonePipe;
}