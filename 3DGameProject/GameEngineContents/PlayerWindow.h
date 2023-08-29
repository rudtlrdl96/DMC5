#pragma once
#include <GameEngineCore/GameEngineGUI.h>
#include "PlayerActor_Nero.h"
// ���� :
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

	static PlayerActor_Nero* _Nero;

protected:
	void OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime) override;
private:

};

