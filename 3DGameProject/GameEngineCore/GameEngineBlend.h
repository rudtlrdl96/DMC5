#pragma once
#include "GameEngineResource.h"

// 설명 :
class GameEngineBlend : public GameEngineResource<GameEngineBlend>
{
public:
	// constrcuter destructer
	GameEngineBlend();
	~GameEngineBlend();

	// delete Function
	GameEngineBlend(const GameEngineBlend& _Other) = delete;
	GameEngineBlend(GameEngineBlend&& _Other) noexcept = delete;
	GameEngineBlend& operator=(const GameEngineBlend& _Other) = delete;
	GameEngineBlend& operator=(GameEngineBlend&& _Other) noexcept = delete;

	static std::shared_ptr<GameEngineBlend> Create(const std::string_view& _Name, const D3D11_BLEND_DESC& _Desc)
	{
		std::shared_ptr<GameEngineBlend> Res = GameEngineResource::Create(_Name);
		Res->ResCreate(_Desc);

		return Res;
	}

	void Setting() override;

protected:
	void ResCreate(const D3D11_BLEND_DESC& _Value);

private:
	D3D11_BLEND_DESC Desc = {};
	ID3D11BlendState* State = nullptr;
	// 최종블랜드 적용 옵션
	//             R G B A
	UINT Mask = 0xFFFFFFFF;

};

