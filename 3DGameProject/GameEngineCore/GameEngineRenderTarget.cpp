#include "PrecompileHeader.h"
#include "GameEngineRenderTarget.h"

GameEngineRenderUnit GameEngineRenderTarget::MergeUnit;

void GameEngineRenderTarget::RenderTargetUnitInit()
{
	MergeUnit.SetMesh("FullRect");
	MergeUnit.SetMaterial("Merge");
}


GameEngineRenderTarget::GameEngineRenderTarget()
{
}

GameEngineRenderTarget::~GameEngineRenderTarget()
{
	DepthTexture = nullptr;
}

// 랜더타겟 생성
void GameEngineRenderTarget::ResCreate(std::shared_ptr<GameEngineTexture> _Texture, float4 _Color)
{
	Color = _Color;                     // 백버퍼의 디폴트 색 지정(우리는 파란색)
	Textures.push_back(_Texture);       // 생성된 텍스쳐 값을 벡터에 푸쉬백
	RTVs.push_back(_Texture->GetRTV()); // 생성된 텍스쳐의 RTV 값을 벡터에 푸쉬백
}

void GameEngineRenderTarget::ResCreate(DXGI_FORMAT _Format, float4 _Scale, float4 _Color)
{
	D3D11_TEXTURE2D_DESC Desc = { 0 };

	Desc.ArraySize = 1;
	Desc.Width = _Scale.uix();
	Desc.Height = _Scale.uiy();
	Desc.Format = _Format;
	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;
	Desc.MipLevels = 1;
	Desc.Usage = D3D11_USAGE_DEFAULT;
	Desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;

	std::shared_ptr<GameEngineTexture> Tex = GameEngineTexture::Create(Desc);
	Textures.push_back(Tex);
	RTVs.push_back(Tex->GetRTV());
}

void GameEngineRenderTarget::CreateDepthTexture(int _Index)
{
	D3D11_TEXTURE2D_DESC Desc = { 0, };

	Desc.ArraySize = 1;
	Desc.Width = Textures[_Index]->GetWidth();
	Desc.Height = Textures[_Index]->GetHeight();
	Desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;    // 4바이트중 3바이트는 0 ~ 1.0f 표현하는데 사용하고, 나머지 1바이트는 스텐실 값으로 사용할 수 있게 해주는 포멧
	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;
	Desc.MipLevels = 1;
	Desc.Usage = D3D11_USAGE_DEFAULT;
	Desc.CPUAccessFlags = 0;
	Desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;

	DepthTexture = GameEngineTexture::Create(Desc); // 뎁스스텐실뷰 생성
}

void GameEngineRenderTarget::Clear()
{
	for (size_t i = 0; i < Textures.size(); i++)
	{
		ID3D11RenderTargetView* RTV = Textures[i]->GetRTV();

		if (nullptr == RTV)
		{
			MsgAssert("랜더타겟 뷰가 존재하지 않아서 클리어가 불가능합니다.");
			return;
		}

		GameEngineDevice::GetContext()->ClearRenderTargetView(RTV, Color.Arr1D);
	}

	ID3D11DepthStencilView* DSV
		= DepthTexture != nullptr ? DepthTexture->GetDSV() : nullptr;

	if (nullptr != DSV)
	{
		GameEngineDevice::GetContext()->ClearDepthStencilView(DSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}

void GameEngineRenderTarget::Setting()
{
	ID3D11RenderTargetView** RTV = &RTVs[0];

	if (nullptr == RTV)
	{
		MsgAssert("랜더타겟 뷰가 존재하지 않아서 클리어가 불가능합니다.");
	}

	ID3D11DepthStencilView* DSV
		= DepthTexture != nullptr ? DepthTexture->GetDSV() : nullptr;

	if (false == DepthSetting)
	{
		DSV = nullptr;
	}

	// 지금 당장은 z값을 쓰지 않겠습니다.
	GameEngineDevice::GetContext()->OMSetRenderTargets(1, RTV, DSV);
}

void GameEngineRenderTarget::Reset()
{
	ID3D11RenderTargetView* RTV[8] = { nullptr };

	GameEngineDevice::GetContext()->OMSetRenderTargets(8, RTV, nullptr);
}

void GameEngineRenderTarget::Merge(std::shared_ptr<GameEngineRenderTarget> _Other, size_t _Index)
{
	Setting();

	MergeUnit.ShaderResHelper.SetTexture("DiffuseTex", _Other->Textures[0]);
	MergeUnit.Render(0.0f);
	MergeUnit.ShaderResHelper.AllResourcesReset();

}

void GameEngineRenderTarget::ReleaseEffect(std::shared_ptr<GameEnginePostProcess> _Effect)
{
	std::vector<std::shared_ptr<GameEnginePostProcess>>::iterator LoopIter = Effects.begin();
	std::vector<std::shared_ptr<GameEnginePostProcess>>::iterator EndIter = Effects.end();

	for (; LoopIter != EndIter; ++LoopIter)
	{
		if ((*LoopIter) == _Effect)
		{
			LoopIter = Effects.erase(LoopIter);
			break;
		}
	}
}

void GameEngineRenderTarget::Effect(float _DeltaTime)
{
	for (size_t i = 0; i < Effects.size(); i++)
	{
		Effects[i]->Effect(this, _DeltaTime);
	}
}

void GameEngineRenderTarget::EffectInit(std::shared_ptr<GameEnginePostProcess> _PostProcess)
{
	_PostProcess->Start(this);
}