#pragma once
#include <GameEngineBase/GameEnginePath.h>
#include <GameEngineCore/GameEngineGUI.h>
#include "FXData.h"
// ���� :
class EffectToolWindow : public GameEngineGUIWindow
{
public:
	// constrcuter destructer
	EffectToolWindow();
	~EffectToolWindow();

	// delete Function
	EffectToolWindow(const EffectToolWindow& _Other) = delete;
	EffectToolWindow(EffectToolWindow&& _Other) noexcept = delete;
	EffectToolWindow& operator=(const EffectToolWindow& _Other) = delete;
	EffectToolWindow& operator=(EffectToolWindow&& _Other) noexcept = delete;

protected:
	void OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime) override;

	void CharacterSetting(std::shared_ptr<GameEngineLevel> Level);
	void CreateFXRender(std::shared_ptr<GameEngineLevel> Level);
	void TimeLine();
private:
	std::shared_ptr<GameEngineActor> Actor = nullptr;
	std::shared_ptr<class GameEngineFBXRenderer> CharacterRender = nullptr;
	std::vector<std::shared_ptr<class EffectRenderer>> FXRenders;
	std::shared_ptr<class FXSystem> FX = nullptr;
	FXData FXValue;

	GameEnginePath FXPath;
	int CurrentFrame = 0;
	bool IsPause = false;

};

