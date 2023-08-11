#pragma once
#include "GameEngineResource.h"

// Ό³Έν :
class GameEngineRasterizer : public GameEngineResource<GameEngineRasterizer>
{
public:
	// constrcuter destructer
	GameEngineRasterizer();
	~GameEngineRasterizer();

	// delete Function
	GameEngineRasterizer(const GameEngineRasterizer& _Other) = delete;
	GameEngineRasterizer(GameEngineRasterizer&& _Other) noexcept = delete;
	GameEngineRasterizer& operator=(const GameEngineRasterizer& _Other) = delete;
	GameEngineRasterizer& operator=(GameEngineRasterizer&& _Other) noexcept = delete;

	static std::shared_ptr<GameEngineRasterizer> Create(const std::string_view& _Name, const D3D11_RASTERIZER_DESC& _Desc)
	{
		std::shared_ptr<GameEngineRasterizer> Res = GameEngineResource::Create(_Name);
		Res->ResCreate(_Desc);

		return Res;
	}

	void Setting() override;

	inline void SetFILL_MODE(D3D11_FILL_MODE _Value)
	{
		switch (_Value)
		{
		case D3D11_FILL_WIREFRAME:
			CurState = WireframeState;
			break;
		case D3D11_FILL_SOLID:
			CurState = SolidState;
			break;
		default:
			break;
		}
	}

	inline void SetCULL_MODE(D3D11_CULL_MODE _Value)
	{
		Desc.CullMode = _Value;
		ResCreate(Desc);
	}

protected:
	void ResCreate(const D3D11_RASTERIZER_DESC& _Value);

private:
	D3D11_RASTERIZER_DESC Desc = {};
	ID3D11RasterizerState* CurState = nullptr;
	ID3D11RasterizerState* SolidState = nullptr;
	ID3D11RasterizerState* WireframeState = nullptr;

	void Release();

};

