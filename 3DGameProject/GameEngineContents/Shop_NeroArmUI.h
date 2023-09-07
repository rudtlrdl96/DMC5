#pragma once
#include <GameEngineCore/GameEngineActor.h>
// ���� :
class Shop_NeroArmUI : public GameEngineActor
{
public:
	// constrcuter destructer
	Shop_NeroArmUI();
	~Shop_NeroArmUI();

	// delete Function
	Shop_NeroArmUI(const Shop_NeroArmUI& _Other) = delete;
	Shop_NeroArmUI(Shop_NeroArmUI&& _Other) noexcept = delete;
	Shop_NeroArmUI& operator=(const Shop_NeroArmUI& _Other) = delete;
	Shop_NeroArmUI& operator=(Shop_NeroArmUI&& _Other) noexcept = delete;

	std::shared_ptr<class Shop_ExplaneUI> GetExPlane()
	{
		return ArmExplane;
	}
protected:
	void Start() override;
	void Update(float _Delta) override;
private:
	void SetIndex();
	void SetExText();
	std::shared_ptr<class Shop_ItemButton>  ArmButton = nullptr;
	std::shared_ptr<class Shop_ExplaneUI> ArmExplane = nullptr;
	bool Test = false;

	int Index = 0;
	int PrevIndex = 2;
	bool Exception = false;
	bool IsValue = false;
	bool DownSwich = false;
	bool UpSwich = false;
	float LerpTime = 0.0f;

	float4 Pos_0 = { -390.0f,234.0f,0.0f };
	float4 Pos_1 = { -390.0f,104.0f,0.0f };
	float4 Pos_2 = { -390.0f,-34.0f,0.0f };
};

