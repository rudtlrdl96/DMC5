#include "PrecompileHeader.h"
#include "GameEngineRenderTarget.h"

GameEngineRenderUnit GameEngineRenderTarget::MergeUnit;
GameEngineRenderUnit GameEngineRenderTarget::CubemapMergeUnit;

void GameEngineRenderTarget::RenderTargetUnitInit()
{
	MergeUnit.SetMesh("FullRect");
	MergeUnit.SetMaterial("Merge");

	CubemapMergeUnit.SetMesh("FullBox");
	CubemapMergeUnit.SetMaterial("CubemapMerge");
}

GameEngineRenderTarget::GameEngineRenderTarget()
{

}

GameEngineRenderTarget::~GameEngineRenderTarget()
{
	DepthTexture = nullptr;
}

void GameEngineRenderTarget::ReleaseTextures()
{
	size_t TexturesSize = Textures.size();

	for (size_t i = 0; i < TexturesSize; i++)
	{
		if (nullptr != Textures[i])
		{
			Textures[i]->Release();
		}
		
		Textures[i] = nullptr;
	}

	Textures.clear();
	RTVs.clear();
	SRVs.clear();
}

// 랜더타겟 생성
void GameEngineRenderTarget::ResCreate(std::shared_ptr<GameEngineTexture> _Texture, float4 _Color)
{
	Color.push_back(_Color);
	D3D11_VIEWPORT ViewPortData;

	ViewPortData.TopLeftX = 0;
	ViewPortData.TopLeftY = 0;
	ViewPortData.Width = _Texture->GetScale().x;
	ViewPortData.Height = _Texture->GetScale().y;
	ViewPortData.MinDepth = 0.0f;
	ViewPortData.MaxDepth = 1.0f;

	ViewPortDatas.push_back(ViewPortData);

	Textures.push_back(_Texture);       // 생성된 텍스쳐 값을 벡터에 푸쉬백
	SRVs.push_back(_Texture->GetSRV());
	RTVs.push_back(_Texture->GetRTV()); // 생성된 텍스쳐의 RTV 값을 벡터에 푸쉬백
}

void GameEngineRenderTarget::ResCreate(DXGI_FORMAT _Format, float4 _Scale, float4 _Color)
{
	Color.push_back(_Color);
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

	D3D11_VIEWPORT ViewPortData;

	ViewPortData.TopLeftX = 0;
	ViewPortData.TopLeftY = 0;
	ViewPortData.Width = _Scale.x;
	ViewPortData.Height = _Scale.y;
	ViewPortData.MinDepth = 0.0f;
	ViewPortData.MaxDepth = 1.0f;

	ViewPortDatas.push_back(ViewPortData);

	Textures.push_back(Tex);
	SRVs.push_back(Tex->GetSRV());
	RTVs.push_back(Tex->GetRTV());
}

void GameEngineRenderTarget::ResCubemapCreate(DXGI_FORMAT _Format, float4 _Scale, float4 _Color)
{
	Color.push_back(_Color);
	D3D11_TEXTURE2D_DESC Desc = { 0 };

	Desc.ArraySize = 6;
	Desc.Width = _Scale.uix();
	Desc.Height = _Scale.uiy();
	Desc.Format = _Format;
	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;
	Desc.MipLevels = 1;
	Desc.Usage = D3D11_USAGE_DEFAULT;
	Desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	Desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;

	D3D11_SHADER_RESOURCE_VIEW_DESC STV;

	STV.Format = Desc.Format;
	STV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	STV.TextureCube.MipLevels = 1;
	STV.TextureCube.MostDetailedMip = 0;

	std::shared_ptr<GameEngineTexture> Tex = GameEngineTexture::CreateCubemap(Desc, STV);

	D3D11_VIEWPORT ViewPortData;

	ViewPortData.TopLeftX = 0;
	ViewPortData.TopLeftY = 0;
	ViewPortData.Width = _Scale.x;
	ViewPortData.Height = _Scale.y;
	ViewPortData.MinDepth = 0.0f;
	ViewPortData.MaxDepth = 1.0f;

	ViewPortDatas.push_back(ViewPortData);

	Textures.push_back(Tex);
	SRVs.push_back(Tex->GetSRV());

	for (size_t i = 0; i < Tex->GetRTVSize(); i++)
	{
		RTVs.push_back(Tex->GetRTV(i));
	}
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
	Desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;

	DepthTexture = GameEngineTexture::Create(Desc); // 뎁스스텐실뷰 생성
}

void GameEngineRenderTarget::Clear()
{
	TextureClear();
	DepthClear();
}

void GameEngineRenderTarget::TextureClear()
{
	for (size_t i = 0; i < Textures.size(); i++)
	{
		for (size_t j = 0; j < Textures[i]->GetRTVSize(); j++)
		{
			ID3D11RenderTargetView* RTV = Textures[i]->GetRTV(j);

			if (nullptr == RTV)
			{
				MsgAssert("랜더타겟 뷰가 존재하지 않아서 클리어가 불가능합니다.");
				return;
			}

			GameEngineDevice::GetContext()->ClearRenderTargetView(RTV, Color[i].Arr1D);
		}
	}
}

void GameEngineRenderTarget::DepthClear()
{
	ID3D11DepthStencilView* DSV = DepthTexture != nullptr ? DepthTexture->GetDSV() : nullptr;

	if (nullptr != DSV)
	{
		GameEngineDevice::GetContext()->ClearDepthStencilView(DSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}

void GameEngineRenderTarget::Setting()
{
	// https://learn.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-resources-limits

	ID3D11RenderTargetView** RTV = &RTVs[0];

	if (nullptr == RTV)
	{
		MsgAssert("랜더타겟 뷰가 존재하지 않아서 클리어가 불가능합니다.");
	}

	ID3D11DepthStencilView* DSV = DepthTexture != nullptr ? DepthTexture->GetDSV() : nullptr;

	if (false == DepthSetting)
	{
		DSV = nullptr;
	}

	GameEngineDevice::GetContext()->OMSetRenderTargets(static_cast<UINT>(RTVs.size()), RTV, DSV);
	GameEngineDevice::GetContext()->RSSetViewports(static_cast<UINT>(ViewPortDatas.size()), &ViewPortDatas[0]);
}

void GameEngineRenderTarget::Setting(size_t _Index)
{
	ID3D11RenderTargetView** RTV = &RTVs[_Index];

	if (nullptr == RTV)
	{
		MsgAssert("랜더타겟 뷰가 존재하지 않아서 클리어가 불가능합니다.");
	}

	ID3D11DepthStencilView* DSV = DepthTexture != nullptr ? DepthTexture->GetDSV() : nullptr;

	if (false == DepthSetting)
	{
		DSV = nullptr;
	}

	GameEngineDevice::GetContext()->OMSetRenderTargets(1, RTV, DSV);
	GameEngineDevice::GetContext()->RSSetViewports(static_cast<UINT>(ViewPortDatas.size()), &ViewPortDatas[0]);
}

void GameEngineRenderTarget::Reset()
{
	GameEngineDevice::GetContext()->OMSetRenderTargets(0, nullptr, nullptr);
}

void GameEngineRenderTarget::Merge(std::shared_ptr<GameEngineRenderTarget> _Other, size_t _Index)
{
	Setting();

	MergeUnit.ShaderResHelper.SetTexture("DiffuseTex", _Other->Textures[_Index]);
	MergeUnit.Render(0.0f);
	MergeUnit.ShaderResHelper.AllResourcesReset();
}

void GameEngineRenderTarget::Merge(GameEngineRenderTarget* _Other, size_t _Index)
{
	Setting();

	MergeUnit.ShaderResHelper.SetTexture("DiffuseTex", _Other->Textures[_Index]);
	MergeUnit.Render(0.0f);
	MergeUnit.ShaderResHelper.AllResourcesReset();
}

void GameEngineRenderTarget::MergeCubemap(std::shared_ptr<GameEngineRenderTarget> _Other)
{
	CubemapMergeUnit.ShaderResHelper.SetTexture("CubemapTex", _Other->Textures[0]);

	for (UINT i = 0; i < 6; i++)
	{
		Setting(i);
		CubemapMergeUnit.Setting();
		CubemapMergeUnit.Draw(6, i * 6);
	}

	CubemapMergeUnit.ShaderResHelper.AllResourcesReset();
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

void GameEngineRenderTarget::ChangeViewPort(float4 _Scale, int _Index)
{
	D3D11_VIEWPORT ViewPortData;
	ViewPortData.TopLeftX = 0;
	ViewPortData.TopLeftY = 0;
	ViewPortData.Width = static_cast<float>(_Scale.uix());
	ViewPortData.Height = static_cast<float>(_Scale.uiy());
	ViewPortData.MinDepth = 0.0f;
	ViewPortData.MaxDepth = 1.0f;

	ViewPortDatas[_Index] = ViewPortData;
}