#pragma once
#include "BaseUIActor.h"
#include <GameEngineCore/GameEngineActor.h>
// Ό³Έν :
class UI_HPGaege : public GameEngineActor
{
public:

public:
	// constrcuter destructer
	UI_HPGaege();
	~UI_HPGaege();

	// delete Function
	UI_HPGaege(const UI_HPGaege& _Other) = delete;
	UI_HPGaege(UI_HPGaege&& _Other) noexcept = delete;
	UI_HPGaege& operator=(const UI_HPGaege& _Other) = delete;
	UI_HPGaege& operator=(UI_HPGaege&& _Other) noexcept = delete;
	std::vector<std::shared_ptr<class UIFBXRenderer>> GetHPFBX()
	{
		return HpGauges;
	}
protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	std::shared_ptr<class UIFBXRenderer> CreateHPGaugeBar(GameEngineLevel* _Level , float4 _Pos);
	std::vector<std::shared_ptr<class UIFBXRenderer>> HpGauges;
};

