#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include "BaseShopUI.h"

// Ό³Έν :
class Vergil_ShopUI : public BaseShopUI
{
public:
	static Vergil_ShopUI* Vergil_ShopBar;
	// constrcuter destructer
	Vergil_ShopUI();
	~Vergil_ShopUI();

	// delete Function
	Vergil_ShopUI(const Vergil_ShopUI& _Other) = delete;
	Vergil_ShopUI(Vergil_ShopUI&& _Other) noexcept = delete;
	Vergil_ShopUI& operator=(const Vergil_ShopUI& _Other) = delete;
	Vergil_ShopUI& operator=(Vergil_ShopUI&& _Other) noexcept = delete;
	int GetUIIndex()
	{
		return Index;
	}
	void ShopOff() override;
	void ShopOn() override;
	void ApperCusterWindow(bool _Value) override;
protected:
	void Start() override;
	void Update(float _Delta) override;


private:
	void SetIndexCount();
	void ScaleUpDown(float _Delta,std::shared_ptr<class Shop_TitleButton> _Button);
	std::shared_ptr<class Shop_TitleButton> SkillButton = nullptr;
	std::shared_ptr<class Shop_TitleButton> YamatoButton = nullptr;
	std::shared_ptr<class Shop_TitleButton> MirgeButton = nullptr;
	std::shared_ptr<class Shop_VergilYamatoUI> YamatoSkill = nullptr;
	std::shared_ptr<class Shop_VergilMirgeUI> MirgeSkill = nullptr;
	std::shared_ptr<class Shop_EnterWindow> EnterWindow = nullptr;

	float LerpTime = 0.0f;
	int ButtonIndex = 0;
	int Index = 0;
	bool IsValue = 0.0f;
	bool ScaleValue = false;
	float Time = 0.0f;
	float4 RenderScale = { 228.0f,62.0f,0.0f };
	float4 RenderTopScale = { 84.0f,23.0f,0.0f };
	float4 RenderBottomScale = { 225.0f,28.0f,0.0f };
};

