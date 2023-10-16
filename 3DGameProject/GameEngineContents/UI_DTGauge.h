#pragma once
#include "UIFBXActorBase.h"

// ���� :
class UI_DTGauge : public UIFBXActorBase
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
	void ActivateDT();
	void CreateDTBar(const std::string_view& _Png , float4 _Pos,float4 _Scale);
	std::shared_ptr<class UIEffectRenderer> DTElectroEffect = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> DTGauge = nullptr;

	std::vector<std::shared_ptr<class UIFBXRenderer>> DTGauges;
	std::vector<std::shared_ptr<class GameEngineUIRenderer>> DTGaugeBars;

	int DTIndex = -1;
};

