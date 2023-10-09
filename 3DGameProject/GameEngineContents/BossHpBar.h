#pragma once
#include <GameEngineCore/GameEngineActor.h>
// Ό³Έν :
#include "UIFBXActorBase.h"
class BossHpBar : public UIFBXActorBase
{
public:
	// constrcuter destructer
	BossHpBar();
	~BossHpBar();

	// delete Function
	BossHpBar(const BossHpBar& _Other) = delete;
	BossHpBar(BossHpBar&& _Other) noexcept = delete;
	BossHpBar& operator=(const BossHpBar& _Other) = delete;
	BossHpBar& operator=(BossHpBar&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _Delta) override;
private:
	std::shared_ptr<class UIFBXRenderer> BossHPUI_Base = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> BossHPUI_Front = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> BossHPUI_HIT = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> BossHPUI_Back = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> BossHpAni = nullptr;
	std::shared_ptr<class UIFBXRenderer> BossFrontGaege = nullptr;
	std::shared_ptr<class UIFBXRenderer> BossHitGauge = nullptr;




};

