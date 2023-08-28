#pragma once
#include <GameEngineCore/GameEngineRenderTarget.h>

class TAA_Effect : public GameEnginePostProcess
{
public:
	TAA_Effect();
	~TAA_Effect();

	TAA_Effect(const TAA_Effect& _Other) = delete;
	TAA_Effect(TAA_Effect&& _Other) noexcept = delete;
	TAA_Effect& operator=(const TAA_Effect& _Other) = delete;
	TAA_Effect& operator=(TAA_Effect&& _Other) noexcept = delete;

protected:
	void Start(GameEngineRenderTarget* _Target) override;
	void Effect(GameEngineRenderTarget* _Target, float _DeltaTime) override;

private:
	std::shared_ptr<GameEngineRenderUnit> TAAUnit = nullptr;
	RenderBaseValue BaseValue = RenderBaseValue();


};

