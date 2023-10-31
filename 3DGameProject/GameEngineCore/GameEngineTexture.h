#pragma once
#include <GameEngineCore/ThirdParty/DirectXTex/inc/DirectXTex.h>
#include "GameEngineResource.h"

class GameEnginePixelColor 
{
public:
	static GameEnginePixelColor Black;

	union 
	{
		struct 
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		};

		unsigned char ColorChar[4];
		int Color;
	};

	bool operator==(GameEnginePixelColor _Color) 
	{
		return Color == _Color.Color;
	}

	float4 Tofloat4() 
	{

	}

	GameEnginePixelColor() 
	{

	}

	GameEnginePixelColor(unsigned char  _r, unsigned char  _g, unsigned char  _b, unsigned char  _a)
		: r(_r), g(_g), b(_b), a(_a)
	{

	}
};

// 설명 : DirectTex 라이브러리 활용
class GameEngineTexture : public GameEngineResource<GameEngineTexture>
{
	friend GameEngineDevice;
	friend class GameEngineTextureSetter;

public:
	// constrcuter destructer
	GameEngineTexture();
	~GameEngineTexture();

	// delete Function
	GameEngineTexture(const GameEngineTexture& _Other) = delete;
	GameEngineTexture(GameEngineTexture&& _Other) noexcept = delete;
	GameEngineTexture& operator=(const GameEngineTexture& _Other) = delete;
	GameEngineTexture& operator=(GameEngineTexture&& _Other) noexcept = delete;

	static std::shared_ptr<GameEngineTexture> Load(const std::string_view& _Path)
	{
		GameEnginePath NewPath(_Path);
		return Load(_Path, NewPath.GetFileName());
	}

	static void PathCheck(const std::string_view& _Path, const std::string_view& _Name);

	static std::shared_ptr<GameEngineTexture> Load(const std::string_view& _Path, const std::string_view& _Name) 
	{
		std::shared_ptr<GameEngineTexture> NewTexture = GameEngineResource::Create(_Name);
		PathCheck(_Path, _Name);
		NewTexture->SetPath(_Path);
		NewTexture->ResLoad(_Path);

		return NewTexture;
	}

	static std::shared_ptr<GameEngineTexture> Create(const std::string_view& _Name, ID3D11Texture2D* _Value)
	{
		std::shared_ptr<GameEngineTexture> NewTexture = GameEngineResource::Create(_Name);
		NewTexture->ResCreate(_Value);

		return NewTexture;
	}

	static std::shared_ptr<GameEngineTexture> Create(const D3D11_TEXTURE2D_DESC& _Value)
	{
		std::shared_ptr<GameEngineTexture> NewTexture = GameEngineResource::CreateUnNamed();
		NewTexture->ResCreate(_Value);

		return NewTexture;
	}

	static std::shared_ptr<GameEngineTexture> CreateCubemap(const D3D11_TEXTURE2D_DESC& _Value, const D3D11_SHADER_RESOURCE_VIEW_DESC& _SRV)
	{
		std::shared_ptr<GameEngineTexture> NewTexture = GameEngineResource::CreateUnNamed();
		NewTexture->ResCreateCubemap(_Value, _SRV);

		return NewTexture;
	}

	static std::shared_ptr<GameEngineTexture> Create(const std::string_view& _Name, std::vector<std::shared_ptr<GameEngineTexture>>& _Textures);

	static std::shared_ptr<GameEngineTexture> UnLoad(const std::string_view& _Name)
	{
		std::shared_ptr<GameEngineTexture> NewTexture = GameEngineResource::Find(_Name);

		if (nullptr == NewTexture)
		{
			return nullptr;
			//MsgAssert("존재하지 않는 텍스처를 언로드 하려고 했습니다.");
		}
		GameEngineResource::Remove(_Name);
		NewTexture->Release();

		return NewTexture;
	}

	static std::shared_ptr<GameEngineTexture> ReLoad(const std::string_view& _Path)
	{
		GameEnginePath NewPath(_Path);

		return ReLoad(_Path, NewPath.GetFileName());
	}

	static std::shared_ptr<GameEngineTexture> ReLoad(const std::string_view& _Path, const std::string_view& _Name)
	{
		std::shared_ptr<GameEngineTexture> NewTexture = GameEngineResource<GameEngineTexture>::Find(_Name);

		if (nullptr == NewTexture)
		{
			MsgAssert("존재하지 않는 텍스처를 로드 하려고 했습니다.");
		}

		NewTexture->ReLoad();

		return NewTexture;
	}

	ID3D11ShaderResourceView* GetSRV()
	{
		return SRV;
	}

	ID3D11RenderTargetView* GetRTV(size_t _Index = 0) 
	{
		return RTVs[_Index];
	}

	inline size_t GetRTVSize() const
	{
		return RTVs.size();
	}

	ID3D11DepthStencilView* GetDSV()
	{
		return DSV;
	}

	ID3D11Texture2D* GetTexture2D()
	{
		return Texture2D;
	}

	int GetWidth() 
	{
		return Desc.Width;
	}

	int GetHeight()
	{
		return Desc.Height;
	}

	DirectX::ScratchImage& GetScratchImage()
	{
		return Image;
	}

	float4 GetScale() 
	{
		return float4(static_cast<float>(Desc.Width), static_cast<float>(Desc.Height));
	}

	GameEnginePixelColor GetPixel(int _X, int _Y, GameEnginePixelColor DefaultColor = GameEnginePixelColor::Black);
	
	void ReLoad();
	void Release();

protected:

private:
	ID3D11Texture2D* Texture2D = nullptr;
	std::vector<ID3D11RenderTargetView*> RTVs;
	ID3D11DepthStencilView* DSV = nullptr;
	ID3D11ShaderResourceView* SRV = nullptr;

	D3D11_TEXTURE2D_DESC Desc;

	DirectX::TexMetadata Data;
	DirectX::ScratchImage Image;

	void ResCreate(ID3D11Texture2D* _Value);
	void ResLoad(const std::string_view& _Path);
	void ResCreate(const D3D11_TEXTURE2D_DESC& _Value);
	void ResCreate(std::vector<std::shared_ptr<GameEngineTexture>>& _Textures, const D3D11_TEXTURE2D_DESC& _Value, const D3D11_RENDER_TARGET_VIEW_DESC& _RTV, const D3D11_SHADER_RESOURCE_VIEW_DESC& _SRV);

	void ResCreateCubemap(const D3D11_TEXTURE2D_DESC& _Value, const D3D11_SHADER_RESOURCE_VIEW_DESC& _SRV);

	void CreateRenderTargetView();
	void CreateRenderTargetView(const D3D11_RENDER_TARGET_VIEW_DESC& _RTVDesc);
	void CreateShaderResourcesView();
	void CreateShaderResourcesView(const D3D11_SHADER_RESOURCE_VIEW_DESC& _SRVDesc);
	void CreateDepthStencilView();

	void VSSetting(UINT _Slot);
	void PSSetting(UINT _Slot);

	void VSReset(UINT _Slot);
	void PSReset(UINT _Slot);
};

