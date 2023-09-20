#pragma once
#include "UIFBXActorBase.h"

// ���� :
class UI_BulletGauge : public UIFBXActorBase
{
public:
	// constrcuter destructer
	UI_BulletGauge();
	~UI_BulletGauge();

	// delete Function
	UI_BulletGauge(const UI_BulletGauge& _Other) = delete;
	UI_BulletGauge(UI_BulletGauge&& _Other) noexcept = delete;
	UI_BulletGauge& operator=(const UI_BulletGauge& _Other) = delete;
	UI_BulletGauge& operator=(UI_BulletGauge&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
private:
	void ClipGauge(float _DeltaTime);
	std::shared_ptr<class GameEngineUIRenderer> BulletGaugeBase= nullptr;
	std::shared_ptr<class GameEngineUIRenderer> BulletGauge = nullptr;

	float AddTime = 0.0f;
	float Time = 0.0f;
	float ExBulletCount = 1.0f;
};

