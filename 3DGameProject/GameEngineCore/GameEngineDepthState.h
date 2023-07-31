#pragma once
#include "GameEngineResource.h"

// Ό³Έν :
class GameEngineDepthState : public GameEngineResource<GameEngineDepthState>
{
public:
	// constrcuter destructer
	GameEngineDepthState();
	~GameEngineDepthState();

	// delete Function
	GameEngineDepthState(const GameEngineDepthState& _Other) = delete;
	GameEngineDepthState(GameEngineDepthState&& _Other) noexcept = delete;
	GameEngineDepthState& operator=(const GameEngineDepthState& _Other) = delete;
	GameEngineDepthState& operator=(GameEngineDepthState&& _Other) noexcept = delete;

	static std::shared_ptr<GameEngineDepthState> Create(const std::string_view& _Name, const D3D11_DEPTH_STENCIL_DESC& _Desc)
	{
		std::shared_ptr<GameEngineDepthState> Res = GameEngineResource::Create(_Name);
		Res->ResCreate(_Desc);
		return Res;
	}

	void Setting() override;

protected:
	void ResCreate(const D3D11_DEPTH_STENCIL_DESC& _Value);

private:
	D3D11_DEPTH_STENCIL_DESC Desc = {};

	ID3D11DepthStencilState* State = nullptr;
};

