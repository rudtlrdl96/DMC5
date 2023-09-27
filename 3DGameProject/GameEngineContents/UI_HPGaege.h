#pragma once
#include "UIFBXActorBase.h"
#include <GameEngineCore/GameEngineActor.h>
// Ό³Έν :
class UI_HPGaege : public UIFBXActorBase
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
	
	void UpdateHPBar(int CurHP)
	{
		CurHp = CurHP;
	};
protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	void ClipHPBar(float _Delta);
	std::shared_ptr<class GameEngineUIRenderer> NeroHPUI_Front = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> NeroHPUI_Back = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> NeroHpAni = nullptr;
	float4 HPAniPos = { -104.0f,404.0f,30.0f };
	int MaxHP = 10000;
	int RedHp = 0;
	int PrevHp = 0;
	int CurHp = 0;
	float Ratio = 0.0f;
};

