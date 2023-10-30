#pragma once
#include <GameEngineCore/GameEngineActor.h>
// ���� :
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
	void UpdateBossHP(int _CurHP)
	{
		CurBossHp = _CurHP;
	};
	void SetBossMaxHP(int _CurMaxHP)
	{
		MaxBossHP = _CurMaxHP;
	};
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
	float4 AniPos = { 462.0f,-398.0f,0.0f };
	float4 AniScale = { 46.0f,170.0f,0.0f };
	float4 Scale = { 906.0f,26.0f,0.0f };
	float4 Pos = { 10.0f,-399.0f,0.0f };
	int MaxBossHP = 0;
	int RedBossHp = 0;
	int PrevBossHp = 0;
	int CurBossHp = 0;
	float Ratio = 0.0f;



};

