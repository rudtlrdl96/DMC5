#pragma once
#include "GaugeBaseActor.h"

// Ό³Έν :
class UI_DTGauge : public GaugeBaseActor
{
public:
	// constrcuter destructer
	UI_DTGauge();
	~UI_DTGauge();

	// delete Function
	UI_DTGauge(const UI_DTGauge& _Other) = delete;
	UI_DTGauge(UI_DTGauge&& _Other) noexcept = delete;
	UI_DTGauge& operator=(const UI_DTGauge& _Other) = delete;
	UI_DTGauge& operator=(UI_DTGauge&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
private:

};

