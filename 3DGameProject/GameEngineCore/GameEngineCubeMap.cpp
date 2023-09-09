#include "PrecompileHeader.h"
#include "GameEngineCubeMap.h"

GameEngineCubeMap::GameEngineCubeMap() 
{
}

GameEngineCubeMap::~GameEngineCubeMap() 
{
}

void GameEngineCubeMap::ResCreate()
{
	D3D11_TEXTURE2D_DESC Desc;

	Desc.Width = 256;  // _Scale.uix();
	Desc.Height = 256;
	Desc.MipLevels = 9;
	Desc.ArraySize = 6;
	Desc.SampleDesc.Quality = 0;
	Desc.SampleDesc.Count = 1;
	Desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	Desc.Usage = D3D11_USAGE_DEFAULT;
	Desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	Desc.CPUAccessFlags = 0;
	Desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE; // 밉맵 자동생성 + 큐브 텍스쳐

	D3D11_RENDER_TARGET_VIEW_DESC DescRTV;

	DescRTV.Format = Desc.Format;
	DescRTV.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	DescRTV.Texture2DArray.ArraySize = 6; // 6면
	DescRTV.Texture2DArray.FirstArraySlice = 0; // 0부터
	DescRTV.Texture2DArray.MipSlice = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC DescSRV;

	DescSRV.Format = Desc.Format;
	DescSRV.ViewDimension = D3D_SRV_DIMENSION_TEXTURECUBE;
	DescSRV.TextureCube.MipLevels = 9;
	DescSRV.TextureCube.MostDetailedMip = 0;

	std::shared_ptr<GameEngineTexture> Tex = GameEngineTexture::CreateCubeTexture(Desc, DescRTV, DescSRV);

	Textures.push_back(Tex);
	SRVs.push_back(Tex->GetSRV());
	RTVs.push_back(Tex->GetRTV());
}

void GameEngineCubeMap::CreateCubeDepthTexture(int _Index)
{
	D3D11_TEXTURE2D_DESC Desc = { 0, };

	Desc.Width = Textures[_Index]->GetWidth();
	Desc.Height = Textures[_Index]->GetHeight();
	//Desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;    // 4바이트중 3바이트는 0 ~ 1.0f 표현하는데 사용하고, 나머지 1바이트는 스텐실 값으로 사용할 수 있게 해주는 포멧
	Desc.Format = DXGI_FORMAT_R32_FLOAT;
	Desc.MipLevels = 1;
	Desc.ArraySize = 6;
	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;
	Desc.Usage = D3D11_USAGE_DEFAULT;
	Desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	Desc.CPUAccessFlags = 0;
	Desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	D3D11_DEPTH_STENCIL_VIEW_DESC DescDSV;

	DescDSV.Format = Desc.Format;
	DescDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	DescDSV.Texture2DArray.FirstArraySlice = 0; // 0부터
	DescDSV.Texture2DArray.ArraySize = 6; // 6면이니까
	DescDSV.Texture2DArray.MipSlice = 0;

	DepthTexture = GameEngineTexture::CreateCubeDepth(Desc, DescDSV); // 뎁스스텐실뷰 생성
}