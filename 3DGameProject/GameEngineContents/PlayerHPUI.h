#pragma once
#include "UIFBXActorBase.h"
// Ό³Έν :
class PlayerHPUI : public UIFBXActorBase
{
public:
	// constrcuter destructer
	PlayerHPUI();
	~PlayerHPUI();

	// delete Function
	PlayerHPUI(const PlayerHPUI& _Other) = delete;
	PlayerHPUI(PlayerHPUI&& _Other) noexcept = delete;
	PlayerHPUI& operator=(const PlayerHPUI& _Other) = delete;
	PlayerHPUI& operator=(PlayerHPUI&& _Other) noexcept = delete;
	void SetPlayerHP(int _HP);
	void ShootBullet();
	void SetExceedCount(int _Ex)
	{
		ExceedCount = _Ex;
	}
	bool IsChargeBullet();
	std::shared_ptr<class UI_DTGauge> GetDtUI()
	{
		return UI_DTGaugeBar;
	}
	void SetVergilUI();
	void SetNeroUI();

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	void CreatFireEffect();
	void SetExceed();
	std::shared_ptr<class UIFBXRenderer> NeroUI_HPGlass = nullptr;
	std::shared_ptr<class UIMeshRenderer> NeroUI_Hud1 = nullptr;
	std::shared_ptr<class UIMeshRenderer> NeroUI_Hud2 = nullptr;
	std::shared_ptr<class UIMeshRenderer> NeroUI_Hud3 = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> ExFire_Effect1 = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> ExFire_Effect2 = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> ExFire_Effect3 = nullptr;

	std::shared_ptr<class GameEngineUIRenderer> RedOrbRender= nullptr;
	std::shared_ptr<class GameEngineFontRenderer> RedOrbFont = nullptr;
	std::shared_ptr<class RedOrbUI> RedOrbUIPtr = nullptr;
	std::shared_ptr<class UI_HPGaege> UI_HPGaegeBar = nullptr;
	std::shared_ptr<class UI_DTGauge> UI_DTGaugeBar = nullptr;
	std::shared_ptr<class UI_BulletGauge> UI_BulletGaugeBar = nullptr;
	int PlayerCurHp = -1;


	int ExceedCount = 0;
	static int RedOrbValue;
	bool IsNero = false;
};

