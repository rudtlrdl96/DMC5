#pragma once
#include <GameEngineBase/GameEnginePath.h>
#include <GameEngineCore/GameEngineGUI.h>
// Ό³Έν :
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

private:

};

