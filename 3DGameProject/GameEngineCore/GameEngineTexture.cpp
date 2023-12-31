#include "PrecompileHeader.h"
#include "GameEngineTexture.h"
#include "GameEngineLevel.h"

#include "..\\DirectXTK\\\WICTextureLoader.h"
#include "..\\DirectXTK\\\DDSTextureLoader.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\GameEngineCore\\ThirdParty\\DirectXTex\\lib\\x64\\Debug\\DirectXTex.lib")
#else
#pragma comment(lib, "..\\GameEngineCore\\ThirdParty\\DirectXTex\\lib\\x64\\Release\\DirectXTex.lib")
#endif

// #pragma comment(lib, "DirectXTex.lib")

GameEnginePixelColor GameEnginePixelColor::Black = {0, 0, 0, 0};

GameEngineTexture::GameEngineTexture() 
{
}

GameEngineTexture::~GameEngineTexture() 
{
	Release();
}

void GameEngineTexture::Release()
{
	Image.Release();

	if (nullptr != DSV)
	{
		DSV->Release();
		DSV = nullptr;
	}

	if (nullptr != SRV)
	{
		SRV->Release();
		SRV = nullptr;
	}

	for (size_t i = 0; i < RTVs.size(); i++)
	{
		if (nullptr != RTVs[i])
		{
			RTVs[i]->Release();
			RTVs[i] = nullptr;
		}
	}

	RTVs.clear();

	if (nullptr != Texture2D)
	{
		Texture2D->Release();
		Texture2D = nullptr;
	}
}

void GameEngineTexture::ReLoad()
{
	if (nullptr != Texture2D)
	{
		return;
	}

	if (nullptr != SRV)
	{
		return;
	}

	if ("" == GetPath())
	{
		return;
	}

	ResLoad(GetPath());
}

// 다이렉트 이니셜라이즈 단계에서, D3D11_TEXTURE2D_DESC에 값 복사한 뒤 RTV 생성
void GameEngineTexture::ResCreate(ID3D11Texture2D* _Create)
{
	Texture2D = _Create;
	Texture2D->GetDesc(&Desc); // Desc(D3D11_TEXTURE2D_DESC)에 값 복사
	CreateRenderTargetView();  // RTV 생성
}

// Desc의 BindFlags에 따라 RTV, SRV, DSV 중 하나를 Create
void GameEngineTexture::ResCreate(const D3D11_TEXTURE2D_DESC& _Value)
{
	Desc = _Value;

	GameEngineDevice::GetDevice()->CreateTexture2D(&Desc, nullptr, &Texture2D);

	if (D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET & Desc.BindFlags)
	{
		CreateRenderTargetView();
	}

	if (D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE & Desc.BindFlags)
	{
		CreateShaderResourcesView();
	}

	if (D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL & Desc.BindFlags)
	{
		CreateDepthStencilView();
	}

	if (nullptr == Texture2D)
	{
		MsgAssert("텍스처 생성에 실패했습니다.");
	}
}

void GameEngineTexture::ResCreateCubemap(const D3D11_TEXTURE2D_DESC& _Value, const D3D11_SHADER_RESOURCE_VIEW_DESC& _SRV)
{
	Desc = _Value;

	GameEngineDevice::GetDevice()->CreateTexture2D(&Desc, nullptr, &Texture2D);

	if (D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET & Desc.BindFlags)
	{
		D3D11_RENDER_TARGET_VIEW_DESC RTV;
		RTV.Format = Desc.Format;
		RTV.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		RTV.Texture2DArray.ArraySize = 1;
		RTV.Texture2DArray.FirstArraySlice = 0;
		RTV.Texture2DArray.MipSlice = 0;

		for (UINT i = 0; i < 6; i++)
		{
			RTV.Texture2DArray.FirstArraySlice = i;
			CreateRenderTargetView(RTV);
		}
	}

	if (D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE & Desc.BindFlags)
	{
		CreateShaderResourcesView(_SRV);
	}

	if (D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL & Desc.BindFlags)
	{
		CreateDepthStencilView();
	}

	if (nullptr == Texture2D)
	{
		MsgAssert("텍스처 생성에 실패했습니다.");
	}
}

void GameEngineTexture::ResCreate(std::vector<std::shared_ptr<GameEngineTexture>>& _Textures, const D3D11_TEXTURE2D_DESC& _Value, const D3D11_RENDER_TARGET_VIEW_DESC& _RTV, const D3D11_SHADER_RESOURCE_VIEW_DESC& _SRV)
{
	Desc = _Value;
	D3D11_RENDER_TARGET_VIEW_DESC DescRTV = _RTV;
	D3D11_SHADER_RESOURCE_VIEW_DESC DescSRV = _SRV;

	//Array to fill which we will use to point D3D at our loaded CPU images.
	D3D11_SUBRESOURCE_DATA pData[6];
	for (int cubeMapFaceIndex = 0; cubeMapFaceIndex < 6; cubeMapFaceIndex++)
	{
		DirectX::ScratchImage& Image = _Textures[cubeMapFaceIndex]->GetScratchImage();

		pData[cubeMapFaceIndex].pSysMem = Image.GetImages()->pixels;
		pData[cubeMapFaceIndex].SysMemPitch = (UINT)Image.GetImages()->rowPitch;
		pData[cubeMapFaceIndex].SysMemSlicePitch = 0;
	}

	//Create the Texture Resource
	HRESULT TextureResult = GameEngineDevice::GetDevice()->CreateTexture2D(&Desc, &pData[0], &Texture2D);
	if (S_OK != TextureResult)
	{
		MsgAssert("큐브 텍스쳐 생성에 실패했습니다.");
		return;
	}

	//HRESULT RTVResult = GameEngineDevice::GetDevice()->CreateRenderTargetView(Texture2D, nullptr, &RTV);

	ID3D11RenderTargetView* NewRTV = nullptr;
	HRESULT RTVResult = GameEngineDevice::GetDevice()->CreateRenderTargetView(Texture2D, &DescRTV, &NewRTV);	
	RTVs.push_back(NewRTV);

	if (S_OK != RTVResult)
	{
		MsgAssert("큐브 랜더타겟 뷰 생성에 실패했습니다.");
		return;
	}

	//If we have created the texture resource for the six faces 
	//we create the Shader Resource View to use in our shaders.
	HRESULT SRVResult = GameEngineDevice::GetDevice()->CreateShaderResourceView(Texture2D, &_SRV, &SRV);
	if (S_OK != SRVResult)
	{
		MsgAssert("큐브 쉐이더 리소스 뷰 생성에 실패했습니다.");
		return;
	}
}

// 랜더타겟뷰(RTV) 생성, CreateRenderTargetView()
void GameEngineTexture::CreateRenderTargetView()
{
	if (nullptr == Texture2D)
	{
		MsgAssert("텍스처가 존재하지 않는 랜더타겟뷰을 만들 수는 없습니다.");
		return;
	}

	ID3D11RenderTargetView* NewRTV = nullptr;

	HRESULT Result = GameEngineDevice::GetDevice()->CreateRenderTargetView(Texture2D, nullptr, &NewRTV);

	if (S_OK != Result)
	{
		MsgAssert("랜더타겟 뷰 생성에 실패했습니다.");
		return;
	}

	RTVs.push_back(NewRTV);
}

void GameEngineTexture::CreateRenderTargetView(const D3D11_RENDER_TARGET_VIEW_DESC& _RTVDesc)
{
	if (nullptr == Texture2D)
	{
		MsgAssert("텍스처가 존재하지 않는 랜더타겟뷰을 만들 수는 없습니다.");
		return;
	}

	ID3D11RenderTargetView* NewRTV = nullptr;

	HRESULT Result = GameEngineDevice::GetDevice()->CreateRenderTargetView(Texture2D, &_RTVDesc, &NewRTV);

	if (S_OK != Result)
	{
		MsgAssert("랜더타겟 뷰 생성에 실패했습니다.");
		return;
	}

	RTVs.push_back(NewRTV);
}

// 쉐이더리소스뷰(SRV) 생성, CreateShaderResourceView()
void GameEngineTexture::CreateShaderResourcesView()
{
	if (nullptr == Texture2D)
	{
		MsgAssert("텍스처가 존재하지 않는 쉐이더 리소스 뷰를 만들 수는 없습니다.");
		return;
	}

	HRESULT Result = GameEngineDevice::GetDevice()->CreateShaderResourceView(Texture2D, nullptr, &SRV);

	if (S_OK != Result)
	{
		MsgAssert("쉐이더 리소스 뷰 생성에 실패했습니다.");
		return;
	}
}

void GameEngineTexture::CreateShaderResourcesView(const D3D11_SHADER_RESOURCE_VIEW_DESC& _SRVDesc)
{
	if (nullptr == Texture2D)
	{
		MsgAssert("텍스처가 존재하지 않는 쉐이더 리소스 뷰를 만들 수는 없습니다.");
		return;
	}

	HRESULT Result = GameEngineDevice::GetDevice()->CreateShaderResourceView(Texture2D, &_SRVDesc, &SRV);

	if (S_OK != Result)
	{
		MsgAssert("쉐이더 리소스 뷰 생성에 실패했습니다.");
		return;
	}
}

// 뎁스스텐실뷰(DSV) 생성, CreateDepthStencilView()
void GameEngineTexture::CreateDepthStencilView()
{
	if (nullptr == Texture2D)
	{
		MsgAssert("텍스처가 존재하지 않는데 뎁스 스텐실 뷰 만들 수는 없습니다.");
		return;
	}

	HRESULT Result = GameEngineDevice::GetDevice()->CreateDepthStencilView(Texture2D, nullptr, &DSV);

	if (S_OK != Result)
	{
		MsgAssert("뎁스 스텐실 뷰 생성에 실패했습니다.");
		return;
	}
}

void GameEngineTexture::ResLoad(const std::string_view& _Path) 
{
	// GameEnginePath NewPath = 

	GameEnginePath NewPath(_Path);

	std::string Ext = GameEngineString::ToUpper(NewPath.GetExtension());
	std::wstring Path = GameEngineString::AnsiToUniCode(NewPath.GetFullPath());
	
	if (Ext == ".TGA")
	{
		if (S_OK != DirectX::LoadFromTGAFile(Path.c_str(), DirectX::TGA_FLAGS_NONE, &Data, Image))
		{
			MsgAssert("TGA 포맷 로드 실패." + std::string(_Path.data()));
		}
	}
	else if (Ext == ".DDS")
	{
		if (S_OK != DirectX::LoadFromDDSFile(Path.c_str(), DirectX::DDS_FLAGS_NONE, &Data, Image))
		{
			MsgAssert("DDS 포맷 로드 실패." + std::string(_Path.data()));
		}
	}
	else if (S_OK != DirectX::LoadFromWICFile(Path.c_str(), DirectX::WIC_FLAGS_NONE, &Data, Image))
	{
		MsgAssert("텍스처 로드에 실패했습니다." + std::string(_Path.data()));
	}

	if (S_OK != DirectX::CreateShaderResourceView
	(
		GameEngineDevice::GetDevice(),
		Image.GetImages(),
		Image.GetImageCount(),
		Image.GetMetadata(),
		&SRV
	))
	{
		MsgAssert("쉐이더 리소스 뷰 생성에 실패했습니다." + std::string(_Path.data()));
	}

	Desc.Width = static_cast<UINT>(Data.width);
	Desc.Height = static_cast<UINT>(Data.height);
	Desc.Format = Data.format;
	Desc.ArraySize = (UINT)Data.arraySize;
	Desc.MiscFlags = (UINT)Data.miscFlags;
	Desc.MipLevels = (UINT)Data.mipLevels;

	// Texture2D->GetDesc(&Desc);
}

void GameEngineTexture::VSSetting(UINT _Slot) 
{
	if (nullptr == SRV)
	{
		MsgAssert("SRV가 존재하지 않는 텍스처를 쉐이더에 세팅할수 없습니다.");
		return;
	}

	GameEngineDevice::GetContext()->VSSetShaderResources(_Slot, 1,&SRV);
}

void GameEngineTexture::PSSetting(UINT _Slot) 
{
	if (nullptr == SRV)
	{
		MsgAssert("SRV가 존재하지 않는 텍스처를 쉐이더에 세팅할수 없습니다.");
		return;
	}

	GameEngineDevice::GetContext()->PSSetShaderResources(_Slot, 1, &SRV);
}

void GameEngineTexture::VSReset(UINT _Slot)
{
	static ID3D11ShaderResourceView* Nullptr = nullptr;

	GameEngineDevice::GetContext()->VSSetShaderResources(_Slot, 1, &Nullptr);
}
void GameEngineTexture::PSReset(UINT _Slot)
{
	static ID3D11ShaderResourceView* Nullptr = nullptr;

	GameEngineDevice::GetContext()->PSSetShaderResources(_Slot, 1, &Nullptr);
}

// 바깥에 나갔다면 무슨색깔 리턴할지에 대한 컬러도 넣어줘야 한다.
GameEnginePixelColor GameEngineTexture::GetPixel(int _X, int _Y, GameEnginePixelColor DefaultColor)
{
	if (0 > _X)
	{
		return DefaultColor;
	}

	if (0 > _Y)
	{
		return DefaultColor;
	}

	if (_X >= GetWidth())
	{
		return DefaultColor;
	}

	if (_Y >= GetHeight())
	{
		return DefaultColor;
	}

	DXGI_FORMAT Fmt = Image.GetMetadata().format;

	// Image.GetImages()->pixels;

	unsigned char* ColorPtr = Image.GetPixels();

	switch (Fmt)
	{
	case DXGI_FORMAT_UNKNOWN:
		break;
	case DXGI_FORMAT_R32G32B32A32_TYPELESS:
		break;
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
		break;
	case DXGI_FORMAT_R32G32B32A32_UINT:
		break;
	case DXGI_FORMAT_R32G32B32A32_SINT:
		break;
	case DXGI_FORMAT_R32G32B32_TYPELESS:
		break;
	case DXGI_FORMAT_R32G32B32_FLOAT:
		break;
	case DXGI_FORMAT_R32G32B32_UINT:
		break;
	case DXGI_FORMAT_R32G32B32_SINT:
		break;
	case DXGI_FORMAT_R16G16B16A16_TYPELESS:
		break;
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
		break;
	case DXGI_FORMAT_R16G16B16A16_UNORM:
		break;
	case DXGI_FORMAT_R16G16B16A16_UINT:
		break;
	case DXGI_FORMAT_R16G16B16A16_SNORM:
		break;
	case DXGI_FORMAT_R16G16B16A16_SINT:
		break;
	case DXGI_FORMAT_R32G32_TYPELESS:
		break;
	case DXGI_FORMAT_R32G32_FLOAT:
		break;
	case DXGI_FORMAT_R32G32_UINT:
		break;
	case DXGI_FORMAT_R32G32_SINT:
		break;
	case DXGI_FORMAT_R32G8X24_TYPELESS:
		break;
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		break;
	case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
		break;
	case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
		break;
	case DXGI_FORMAT_R10G10B10A2_TYPELESS:
		break;
	case DXGI_FORMAT_R10G10B10A2_UNORM:
		break;
	case DXGI_FORMAT_R10G10B10A2_UINT:
		break;
	case DXGI_FORMAT_R11G11B10_FLOAT:
		break;
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
		break;
	case DXGI_FORMAT_R8G8B8A8_UNORM:
		break;
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
		break;
	case DXGI_FORMAT_R8G8B8A8_UINT:
		break;
	case DXGI_FORMAT_R8G8B8A8_SNORM:
		break;
	case DXGI_FORMAT_R8G8B8A8_SINT:
		break;
	case DXGI_FORMAT_R16G16_TYPELESS:
		break;
	case DXGI_FORMAT_R16G16_FLOAT:
		break;
	case DXGI_FORMAT_R16G16_UNORM:
		break;
	case DXGI_FORMAT_R16G16_UINT:
		break;
	case DXGI_FORMAT_R16G16_SNORM:
		break;
	case DXGI_FORMAT_R16G16_SINT:
		break;
	case DXGI_FORMAT_R32_TYPELESS:
		break;
	case DXGI_FORMAT_D32_FLOAT:
		break;
	case DXGI_FORMAT_R32_FLOAT:
		break;
	case DXGI_FORMAT_R32_UINT:
		break;
	case DXGI_FORMAT_R32_SINT:
		break;
	case DXGI_FORMAT_R24G8_TYPELESS:
		break;
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
		break;
	case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
		break;
	case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
		break;
	case DXGI_FORMAT_R8G8_TYPELESS:
		break;
	case DXGI_FORMAT_R8G8_UNORM:
		break;
	case DXGI_FORMAT_R8G8_UINT:
		break;
	case DXGI_FORMAT_R8G8_SNORM:
		break;
	case DXGI_FORMAT_R8G8_SINT:
		break;
	case DXGI_FORMAT_R16_TYPELESS:
		break;
	case DXGI_FORMAT_R16_FLOAT:
		break;
	case DXGI_FORMAT_D16_UNORM:
		break;
	case DXGI_FORMAT_R16_UNORM:
		break;
	case DXGI_FORMAT_R16_UINT:
		break;
	case DXGI_FORMAT_R16_SNORM:
		break;
	case DXGI_FORMAT_R16_SINT:
		break;
	case DXGI_FORMAT_R8_TYPELESS:
		break;
	case DXGI_FORMAT_R8_UNORM:
		break;
	case DXGI_FORMAT_R8_UINT:
		break;
	case DXGI_FORMAT_R8_SNORM:
		break;
	case DXGI_FORMAT_R8_SINT:
		break;
	case DXGI_FORMAT_A8_UNORM:
		break;
	case DXGI_FORMAT_R1_UNORM:
		break;
	case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
		break;
	case DXGI_FORMAT_R8G8_B8G8_UNORM:
		break;
	case DXGI_FORMAT_G8R8_G8B8_UNORM:
		break;
	case DXGI_FORMAT_BC1_TYPELESS:
		break;
	case DXGI_FORMAT_BC1_UNORM:
		break;
	case DXGI_FORMAT_BC1_UNORM_SRGB:
		break;
	case DXGI_FORMAT_BC2_TYPELESS:
		break;
	case DXGI_FORMAT_BC2_UNORM:
		break;
	case DXGI_FORMAT_BC2_UNORM_SRGB:
		break;
	case DXGI_FORMAT_BC3_TYPELESS:
		break;
	case DXGI_FORMAT_BC3_UNORM:
		break;
	case DXGI_FORMAT_BC3_UNORM_SRGB:
		break;
	case DXGI_FORMAT_BC4_TYPELESS:
		break;
	case DXGI_FORMAT_BC4_UNORM:
		break;
	case DXGI_FORMAT_BC4_SNORM:
		break;
	case DXGI_FORMAT_BC5_TYPELESS:
		break;
	case DXGI_FORMAT_BC5_UNORM:
		break;
	case DXGI_FORMAT_BC5_SNORM:
		break;
	case DXGI_FORMAT_B5G6R5_UNORM:
		break;
	case DXGI_FORMAT_B5G5R5A1_UNORM:
		break;
	case DXGI_FORMAT_B8G8R8A8_UNORM:
	{
		int Index = _Y * static_cast<int>(GetWidth()) + _X;
		ColorPtr = ColorPtr + (Index * 4);
		GameEnginePixelColor Return;
		Return.r = ColorPtr[2];
		Return.g = ColorPtr[1];
		Return.b = ColorPtr[0];
		Return.a = ColorPtr[3];
		return Return;
	}
	case DXGI_FORMAT_B8G8R8X8_UNORM:
	{
		// 컬러1개에 4바이트인 100 * 100
		// 10, 10
		int Index = _Y * static_cast<int>(GetWidth()) + _X;
		ColorPtr = ColorPtr + (Index * 4);
		GameEnginePixelColor Return;
		Return.r = ColorPtr[2];
		Return.g = ColorPtr[1];
		Return.b = ColorPtr[0];
		Return.a = ColorPtr[3];
		return Return;
	}
	case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
		break;
	case DXGI_FORMAT_B8G8R8A8_TYPELESS:
		break;
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
	{
		// 컬러1개에 4바이트인 100 * 100
		// 10, 10
		int Index = _Y * static_cast<int>(GetWidth()) + _X;
		ColorPtr = ColorPtr + (Index * 4);
		GameEnginePixelColor Return;
		Return.r = ColorPtr[2];
		Return.g = ColorPtr[1];
		Return.b = ColorPtr[0];
		Return.a = ColorPtr[3];
		return Return;
	}
	case DXGI_FORMAT_B8G8R8X8_TYPELESS:
		break;
	case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
		break;
	case DXGI_FORMAT_BC6H_TYPELESS:
		break;
	case DXGI_FORMAT_BC6H_UF16:
		break;
	case DXGI_FORMAT_BC6H_SF16:
		break;
	case DXGI_FORMAT_BC7_TYPELESS:
		break;
	case DXGI_FORMAT_BC7_UNORM:
		break;
	case DXGI_FORMAT_BC7_UNORM_SRGB:
		break;
	case DXGI_FORMAT_AYUV:
		break;
	case DXGI_FORMAT_Y410:
		break;
	case DXGI_FORMAT_Y416:
		break;
	case DXGI_FORMAT_NV12:
		break;
	case DXGI_FORMAT_P010:
		break;
	case DXGI_FORMAT_P016:
		break;
	case DXGI_FORMAT_420_OPAQUE:
		break;
	case DXGI_FORMAT_YUY2:
		break;
	case DXGI_FORMAT_Y210:
		break;
	case DXGI_FORMAT_Y216:
		break;
	case DXGI_FORMAT_NV11:
		break;
	case DXGI_FORMAT_AI44:
		break;
	case DXGI_FORMAT_IA44:
		break;
	case DXGI_FORMAT_P8:
		break;
	case DXGI_FORMAT_A8P8:
		break;
	case DXGI_FORMAT_B4G4R4A4_UNORM:
		break;
	case DXGI_FORMAT_P208:
		break;
	case DXGI_FORMAT_V208:
		break;
	case DXGI_FORMAT_V408:
		break;
	case DXGI_FORMAT_SAMPLER_FEEDBACK_MIN_MIP_OPAQUE:
		break;
	case DXGI_FORMAT_SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE:
		break;
	case DXGI_FORMAT_FORCE_UINT:
		break;
	default:
		break;
	}

	return DefaultColor;
}

void GameEngineTexture::PathCheck(const std::string_view& _Path, const std::string_view& _Name)
{
	if (nullptr == GameEngineCore::CurLoadLevel)
	{
		return;
	}
	GameEngineCore::CurLoadLevel->TexturePath[_Name.data()] = _Path.data();
}

std::shared_ptr<GameEngineTexture> GameEngineTexture::Create(const std::string_view& _Name, std::vector<std::shared_ptr<GameEngineTexture>>& _Textures)
{
	D3D11_TEXTURE2D_DESC Desc = { 0 };

	UINT Size = _Textures[0]->GetScale().ix();

	Desc.ArraySize = 6;
	Desc.Width = Size;
	Desc.Height = Size;
	Desc.Format = _Textures[0]->Desc.Format;
	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;
	Desc.MipLevels = 1;
	Desc.Usage = D3D11_USAGE_DEFAULT;
	Desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	Desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;

	D3D11_RENDER_TARGET_VIEW_DESC RTV;
	RTV.Format = Desc.Format;
	RTV.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	RTV.Texture2DArray.ArraySize = 6;
	RTV.Texture2DArray.FirstArraySlice = 0;
	RTV.Texture2DArray.MipSlice = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC STV;

	STV.Format = Desc.Format;
	STV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	STV.TextureCube.MipLevels = 1;
	STV.TextureCube.MostDetailedMip = 0;

	std::shared_ptr<GameEngineTexture> NewTexture = GameEngineResource::Create(_Name);
	NewTexture->ResCreate(_Textures, Desc, RTV, STV);

	return NewTexture;
}
