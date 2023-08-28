#pragma once
#include <GameEngineCore/GameEngineGUI.h>

// Ό³Έν :
class PlayerWindow : public GameEngineGUIWindow
{
public:
	// constrcuter destructer
	PlayerWindow();
	~PlayerWindow();

	// delete Function
	PlayerWindow(const PlayerWindow& _Other) = delete;
	PlayerWindow(PlayerWindow&& _Other) noexcept = delete;
	PlayerWindow& operator=(const PlayerWindow& _Other) = delete;
	PlayerWindow& operator=(PlayerWindow&& _Other) noexcept = delete;

	std::vector<std::vector<std::shared_ptr<GameEngineRenderUnit>>>* RenderUnits = nullptr;

protected:
	void OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime) override;
private:

};

