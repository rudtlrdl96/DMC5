#pragma once
#include "UIFBXActorBase.h"
// Ό³Έν :
class NeroHPUI : public UIFBXActorBase
{
public:

public:
	// constrcuter destructer
	NeroHPUI();
	~NeroHPUI();

	// delete Function
	NeroHPUI(const NeroHPUI& _Other) = delete;
	NeroHPUI(NeroHPUI&& _Other) noexcept = delete;
	NeroHPUI& operator=(const NeroHPUI& _Other) = delete;
	NeroHPUI& operator=(NeroHPUI&& _Other) noexcept = delete;
	void SetPlayerHP(int _HP);
	void ShootBullet();
	void SetExceedCount(int _Ex)
	{
		ExceedCount = _Ex;
	}
	bool IsChargeBullet();

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	void CreatFireEffect();
	std::shared_ptr<class UIFBXRenderer> NeroUI_HPGlass = nullptr;
	std::shared_ptr<class UIFBXRenderer> NeroUI_Hud1 = nullptr;
	std::shared_ptr<class UIFBXRenderer> NeroUI_Hud2 = nullptr;
	std::shared_ptr<class UIFBXRenderer> NeroUI_Hud3 = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> ExFire_Effect1 = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> ExFire_Effect2 = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> ExFire_Effect3 = nullptr;
	std::shared_ptr<class UI_HPGaege> UI_HPGaegeBar = nullptr;
	std::shared_ptr<class UI_DTGauge> UI_DTGaugeBar = nullptr;
	std::shared_ptr<class UI_BulletGauge> UI_BulletGaugeBar = nullptr;
	int PlayerCurHp = -1;


	int ExceedCount = 0;

};

