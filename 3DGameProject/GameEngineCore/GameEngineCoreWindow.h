#pragma once
#include "GameEngineGUI.h"
#include "GameEngineLevel.h"

// Ό³Έν :
class GameEngineCoreWindow : public GameEngineGUIWindow
{
public:
	// constrcuter destructer
	GameEngineCoreWindow();
	~GameEngineCoreWindow();

	// delete Function
	GameEngineCoreWindow(const GameEngineCoreWindow& _Other) = delete;
	GameEngineCoreWindow(GameEngineCoreWindow&& _Other) noexcept = delete;
	GameEngineCoreWindow& operator=(const GameEngineCoreWindow& _Other) = delete;
	GameEngineCoreWindow& operator=(GameEngineCoreWindow&& _Other) noexcept = delete;

	std::function<void()> Test;

	void OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime) override;

protected:

private:
};

