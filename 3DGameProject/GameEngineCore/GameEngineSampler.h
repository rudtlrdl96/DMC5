#pragma once
#include "GameEngineResource.h"

// 설명 :
class GameEngineSampler : public GameEngineResource<GameEngineSampler>
{
	friend class GameEngineSamplerSetter;

public:
	// constrcuter destructer
	GameEngineSampler();
	~GameEngineSampler();

	// delete Function
	GameEngineSampler(const GameEngineSampler& _Other) = delete;
	GameEngineSampler(GameEngineSampler&& _Other) noexcept = delete;
	GameEngineSampler& operator=(const GameEngineSampler& _Other) = delete;
	GameEngineSampler& operator=(GameEngineSampler&& _Other) noexcept = delete;

	static std::shared_ptr<GameEngineSampler> Create(const std::string_view& _Name, const D3D11_SAMPLER_DESC& Desc)
	{
		std::shared_ptr<GameEngineSampler> NewSampler = GameEngineResource::Create(_Name);
		NewSampler->ResCreate(Desc);
		return NewSampler;
	}

	static std::shared_ptr<GameEngineSampler> ReSetting(const std::string_view& _Name, const D3D11_SAMPLER_DESC& Desc)
	{
		 std::shared_ptr<GameEngineSampler> NewSampler = GameEngineResource::Find(_Name);
		// NewTexture->ResCreate(Desc);

		if (nullptr == NewSampler)
		{
			MsgAssert("존재하지 않는 샘플러의 옵션을 변경하려고 했습니다.");
			return nullptr;
		}

		NewSampler->ResCreate(Desc);

		return NewSampler;
	}


protected:

private:
	ID3D11SamplerState* State = nullptr;
	D3D11_SAMPLER_DESC Desc;

	void ResCreate(const D3D11_SAMPLER_DESC& _Desc);

	void VSSetting(UINT _Slot);
	void PSSetting(UINT _Slot);
};

