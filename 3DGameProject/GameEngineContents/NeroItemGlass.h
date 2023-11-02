#pragma once
#include "UIFBXActorBase.h"
#include "ContentsEnum.h"
#include <GameEngineCore/GameEngineFSM.h>
enum FSM_UIMOVEITEM_STATE
{
	UI_GetArm_Reduce,
	UI_GetArm_Turn,
	UI_GetArm_Grow,
	UI_BrokeArm_Reduce,
	UI_BrokeArm_Turn,
	UI_BrokeArm_Grow
};
// 설명 :
class NeroItemGlass : public UIFBXActorBase
{
public:

public:
	// constrcuter destructer
	NeroItemGlass();
	~NeroItemGlass();

	// delete Function
	NeroItemGlass(const NeroItemGlass& _Other) = delete;
	NeroItemGlass(NeroItemGlass&& _Other) noexcept = delete;
	NeroItemGlass& operator=(const NeroItemGlass& _Other) = delete;
	NeroItemGlass& operator=(NeroItemGlass&& _Other) noexcept = delete;
	static void AddItemUI(bool _Value)
	{
		AddItemValue = _Value;
	}
	static void DestroyItemUI(bool _Value)
	{
		DestroyItemValue = _Value;
	}
	void ArmRenderOff();
	void ArmRenderOn();
	void SetDevilBreakerUI();


protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	void SetItemText();
	void AddDestroyValue();
	float4 BezierTransform(const float4& _Start, const float4& _Height, const float4& _End, float _Ratio);
	std::shared_ptr<class UIFBXRenderer> NeroUI_ItemGlass = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> ItemText = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> CurCount = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> SlashText = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> MaxCount = nullptr;
	DevilBreaker CurDevilBreaker = DevilBreaker::None;
	std::vector<DevilBreaker> DevilBreakers;
	std::list<DevilBreaker>* ArmList;
	int MaxItem = 4;
	float Time = 0.0f;
	bool IsOff = false;
	//데빌 브레이커 트렌스폼
	float4 FirstScale = { 5.0f,5.0f,5.0f };
	float4 SecondScale = { 4.0f,4.0f,4.0f };
	float4 ThirdScale = { 3.0f,3.0f,3.0f };
	float4 StartPos = { 730.0f,-300.0f,100.0f };
	float4 EndPos = { 635.0f,-300.0f,100.0f };
	int i = 0;
	// 손돌리기
	bool AddFirst = false;
	bool AddSecound = false;
	bool DestroyFirst = false;
	bool DestroySecond = false;
	float Ratio = 1.0f;
	static bool AddItemValue;
	static bool DestroyItemValue;
	void AddMoveBreaker(float _Delta);
	void DestroyMoveBreaker(float _Delta);

	std::vector<std::shared_ptr<class UIMeshRenderer>> Arms;
	std::shared_ptr<class UIMeshRenderer> Render = nullptr;

	std::shared_ptr<class UIMeshRenderer> Render_0 = nullptr;
	std::shared_ptr<class UIMeshRenderer> Render_1 = nullptr;
	std::shared_ptr<class UIMeshRenderer> Render_2= nullptr;

};

