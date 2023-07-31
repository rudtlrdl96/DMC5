#include "PrecompileHeader.h"
#include "GameEngineRasterizer.h"

GameEngineRasterizer::GameEngineRasterizer() 
{
}

GameEngineRasterizer::~GameEngineRasterizer() 
{
	Release();
}

void GameEngineRasterizer::Setting() 
{
	if (nullptr == CurState)
	{
		MsgAssert("생성되지 않은 레스터라이저 스테이트 입니다.");
	}

	GameEngineDevice::GetContext()->RSSetState(CurState);
}

void GameEngineRasterizer::Release()
{
	if (nullptr != SolidState)
	{
		SolidState->Release();
		SolidState = nullptr;
	}

	if (nullptr != WireframeState)
	{
		WireframeState->Release();
		WireframeState = nullptr;
	}
}

void GameEngineRasterizer::ResCreate(const D3D11_RASTERIZER_DESC& _Value) 
{
	Release();
	Desc = _Value;

	Desc.FillMode = D3D11_FILL_WIREFRAME;
	if (S_OK != GameEngineDevice::GetDevice()->CreateRasterizerState(&Desc, &WireframeState))
	{
		MsgAssert("와이어 프레임 레스터라이저 스테이트 생성에 실패했습니다.");
	}

	Desc.FillMode = D3D11_FILL_SOLID;
	if (S_OK != GameEngineDevice::GetDevice()->CreateRasterizerState(&Desc, &SolidState))
	{
		MsgAssert("솔리드 레스터라이저 스테이트 생성에 실패했습니다.");
	}

	SetFILL_MODE(D3D11_FILL_SOLID);
}