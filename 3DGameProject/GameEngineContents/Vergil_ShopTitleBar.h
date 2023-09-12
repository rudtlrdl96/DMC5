#pragma once
#include <GameEngineCore/GameEngineActor.h>
// Ό³Έν :
class Vergil_ShopTitleBar : public GameEngineActor
{
public:
	static Vergil_ShopTitleBar* Vergil_ShopBar;
	// constrcuter destructer
	Vergil_ShopTitleBar();
	~Vergil_ShopTitleBar();

	// delete Function
	Vergil_ShopTitleBar(const Vergil_ShopTitleBar& _Other) = delete;
	Vergil_ShopTitleBar(Vergil_ShopTitleBar&& _Other) noexcept = delete;
	Vergil_ShopTitleBar& operator=(const Vergil_ShopTitleBar& _Other) = delete;
	Vergil_ShopTitleBar& operator=(Vergil_ShopTitleBar&& _Other) noexcept = delete;
	int GetUIIndex()
	{
		return Index;
	}
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

