#pragma once
#include "GaugeBaseActor.h"
#include <GameEngineCore/GameEngineActor.h>
// Ό³Έν :
class UI_HPGaege : public GaugeBaseActor
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

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
};

