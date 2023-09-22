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
protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	void SetItemText();
	void MoveItem(float _Delta);
	GameEngineFSM FSM;

	std::shared_ptr<class UIFBXRenderer> NeroUI_ItemGlass = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> ItemText = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> CurCount = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> SlashText = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> MaxCount = nullptr;
	DevilBreaker CurDevilBreaker = DevilBreaker::None;
	DevilBreaker RecentDevilBreaker = DevilBreaker::None;

	std::list<DevilBreaker>* ArmList;
	int MaxItem = 4;
	float Time = 0.0f;
	bool first = false;
	bool secound = false;
	bool third = false;

	static bool AddItemValue;
	int i = 0;
	// 손돌리기
	void MoveBreaker(float _Delta);
	std::vector<std::shared_ptr<class UIFBXRenderer>> Arms;
	std::shared_ptr<class UIFBXRenderer> Render = nullptr;
};

