#include "PrecompileHeader.h"
#include "GameEngineRenderingPipeLine.h"
#include <GameEngineCore/GameEngineDevice.h>
#include "GameEngineVertexBuffer.h"
#include "GameEngineIndexBuffer.h"
#include "GameEngineVertexShader.h"
#include "GameEngineRasterizer.h"
#include "GameEnginePixelShader.h"
#include "GameEngineBlend.h"
#include "GameEngineInputLayOut.h"
#include "GameEngineDepthState.h"

GameEngineRenderingPipeLine::GameEngineRenderingPipeLine() 
{
	// InputLayOutPtr = std::make_shared<GameEngineInputLayOut>();
}

GameEngineRenderingPipeLine::~GameEngineRenderingPipeLine() 
{
}

// 매쉬 + 머티리얼
//
//// 점에 대한 정보를 준비하고
//void GameEngineRenderingPipeLine::InputAssembler1() 
//{
//	if (nullptr == InputLayOutPtr)
//	{
//		MsgAssert("인풋 레이아웃이 존재하지 않아서 인풋어셈블러1 과정을 실행할 수 없습니다.");
//		return;
//	}
//
//	// 랜더링을 위해서는 점으로 면을 만드는게 기본이다.
//	// 그래픽카드는 점이 어떠한 구성을 가지고 있는지 알지 못한다.
//	InputLayOutPtr->Setting();
//
//	if (nullptr == VertexBufferPtr)
//	{
//		MsgAssert("버텍스 버퍼가 존재하지 않아서 인풋어셈블러1 과정을 실행할 수 없습니다.");
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
// 로컬에 존재하는 점을 우리가 만든 행렬을 통해서 변환하고.
void GameEngineRenderingPipeLine::VertexShader()
{
	if (nullptr == VertexShaderPtr)
	{
		MsgAssert("버텍스 쉐이더가 존재하지 않아서 버텍스 쉐이더 과정을 실행할 수 없습니다.");
		return;
	}

	VertexShaderPtr->Setting();
}
//
//// 점의 정보를 토대로 어떤 순서로 그릴지 정하고
//void GameEngineRenderingPipeLine::InputAssembler2() 
//{
//	// 그리는 순서에 대한 데이터를 넣어준다.
//	if (nullptr == IndexBufferPtr)
//	{
//		MsgAssert("인덱스 버퍼가 존재하지 않아서 인풋 어셈블러2 과정을 실행할 수 없습니다.");
//		return;
//	}
//
//	// 012023
//	IndexBufferPtr->Setting();
//
//	// 012 023 <=3개씩 끊어서 면으로 만들어라는 여기서 처리가 되었다.
//	GameEngineDevice::GetContext()->IASetPrimitiveTopology(TOPOLOGY);
//}

// 여기서부터
void GameEngineRenderingPipeLine::HullShader() 
{

}
void GameEngineRenderingPipeLine::Tessellator() 
{

}
void GameEngineRenderingPipeLine::DomainShader() 
{

}
void GameEngineRenderingPipeLine::GeometryShaeder() 
{

}
// 여기까지는 화면과는 사실 관련 없음

// w나누기를 해줍니다. 
// 뷰포트도 곱해줍니다.
// 화면 컬링 
// 픽셀 건지기
void GameEngineRenderingPipeLine::Rasterizer() 
{
	if (nullptr == RasterizerPtr)
	{
		MsgAssert("레스터라이저가 존재하지 않아서 세팅이 불가능합니다.");
		return;
	}

	RasterizerPtr->SetFILL_MODE(FILL_MODE);
	RasterizerPtr->Setting();

	// GameEngineDevice::GetContext()->RSSetState
}


void GameEngineRenderingPipeLine::PixelShader() 
{
	if (nullptr == PixelShaderPtr)
	{
		MsgAssert("픽셀 쉐이더가 존재하지 않아서 픽셀 쉐이더 과정을 실행할 수 없습니다.");
		return;
	}

	PixelShaderPtr->Setting();


	// GameEngineDevice::GetContext()->PSSetShader
}
void GameEngineRenderingPipeLine::OutputMerger() 
{
	if (nullptr == BlendStatePtr)
	{
		MsgAssert("블랜드가 존재하지 않아 아웃풋 머저 과정을 완료할수가 없습니다.");
		return;
	}


	BlendStatePtr->Setting();

	if (nullptr == DepthStatePtr)
	{
		MsgAssert("블랜드가 존재하지 않아 아웃풋 머저 과정을 완료할수가 없습니다.");
		return;
	}
	DepthStatePtr->Setting();
}

//
//void GameEngineRenderingPipeLine::SetVertexBuffer(const std::string_view& _Value)
//{
//	std::string UpperName = GameEngineString::ToUpper(_Value);
//	VertexBufferPtr = GameEngineVertexBuffer::Find(UpperName);
//
//	if (nullptr == VertexBufferPtr)
//	{
//		MsgAssert("존재하지 않는 버텍스 버퍼를 사용하려고 했습니다.");
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
//void GameEngineRenderingPipeLine::SetIndexBuffer(const std::string_view& _Value)
//{
//	std::string UpperName = GameEngineString::ToUpper(_Value);
//	IndexBufferPtr = GameEngineIndexBuffer::Find(UpperName);
//
//	if (nullptr == IndexBufferPtr)
//	{
//		MsgAssert("존재하지 않는 버텍스 버퍼를 사용하려고 했습니다.");
//	}
//}



void GameEngineRenderingPipeLine::SetVertexShader(const std::string_view& _Value)
{
	std::string UpperName = GameEngineString::ToUpper(_Value);
	VertexShaderPtr = GameEngineVertexShader::Find(UpperName);

	if (nullptr == VertexShaderPtr)
	{
		MsgAssert("존재하지 않는 버텍스 쉐이더를 사용하려고 했습니다.");
	}

	if (nullptr == VertexBufferPtr)
	{
		return;
	}

	// InputLayOutPtr->ResCreate(VertexBufferPtr, VertexShaderPtr);
}


void GameEngineRenderingPipeLine::SetPixelShader(const std::string_view& _Value)
{
	std::string UpperName = GameEngineString::ToUpper(_Value);
	PixelShaderPtr = GameEnginePixelShader::Find(UpperName);

	if (nullptr == PixelShaderPtr)
	{
		MsgAssert("존재하지 않는 픽셀 쉐이더를 사용하려고 했습니다.");
	}
}

void GameEngineRenderingPipeLine::SetBlendState(const std::string_view& _Value)
{
	std::string UpperName = GameEngineString::ToUpper(_Value);
	BlendStatePtr = GameEngineBlend::Find(UpperName);

	if (nullptr == BlendStatePtr)
	{
		MsgAssert("존재하지 않는 블랜드를 세팅하려고 했습니다.");
		return;
	}
}

void GameEngineRenderingPipeLine::SetDepthState(const std::string_view& _Value)
{
	std::string UpperName = GameEngineString::ToUpper(_Value);
	DepthStatePtr = GameEngineDepthState::Find(UpperName);

	if (nullptr == DepthStatePtr)
	{
		MsgAssert("존재하지 않는 깊이버퍼 스테이트를 세팅하려고 했습니다.");
		return;
	}
}

void GameEngineRenderingPipeLine::SetRasterizer(const std::string_view& _Value)
{
	std::string UpperName = GameEngineString::ToUpper(_Value);
	RasterizerPtr = GameEngineRasterizer::Find(UpperName);

	if (nullptr == RasterizerPtr)
	{
		MsgAssert("존재하지 않는 레스터라이저를 사용하려고 했습니다.");
	}
}

void GameEngineRenderingPipeLine::RenderingPipeLineSetting()
{
	// 랜더라고 하는 부분은 랜더링 파이프라인을 한바뀌 돌리는 것.
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

void GameEngineRenderingPipeLine::Render()
{
	//UINT IndexCount = IndexBufferPtr->GetIndexCount();
	//GameEngineDevice::GetContext()->DrawIndexed(IndexCount, 0, 0);
}

std::shared_ptr<GameEngineRenderingPipeLine> GameEngineRenderingPipeLine::Clone() 
{
	std::shared_ptr<GameEngineRenderingPipeLine> ClonePipe = std::make_shared<GameEngineRenderingPipeLine>();

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