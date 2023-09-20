#pragma once
#include "UIFBXActorBase.h"
#include "ContentsEnum.h"

// Ό³Έν :
class NeroItemGlass : public UIFBXActorBase, public GameEngineNetObject
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

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	void SetItemText();
	std::shared_ptr<class UIFBXRenderer> NeroUI_ItemGlass = nullptr;

	std::shared_ptr<class UIFBXRenderer> NeroUI_Overture = nullptr;
	std::shared_ptr<class UIFBXRenderer> NeroUI_Gerbera = nullptr;
	std::shared_ptr<class UIFBXRenderer> NeroUI_BusterArm = nullptr;

	std::shared_ptr<class GameEngineFontRenderer> ItemText = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> CurCount = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> SlashText = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> MaxCount = nullptr;
	DevilBreaker CurDevilBreaker = DevilBreaker::None;
	int MaxItem = 4;


};

