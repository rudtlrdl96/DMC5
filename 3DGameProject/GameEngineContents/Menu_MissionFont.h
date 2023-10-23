#pragma once
#include <GameEngineCore/GameEngineActor.h>
// Ό³Έν :
class Menu_MissionFont :public GameEngineActor

{
public:
	// constrcuter destructer
	Menu_MissionFont();
	~Menu_MissionFont();

	// delete Function
	Menu_MissionFont(const Menu_MissionFont& _Other) = delete;
	Menu_MissionFont(Menu_MissionFont&& _Other) noexcept = delete;
	Menu_MissionFont& operator=(const Menu_MissionFont& _Other) = delete;
	Menu_MissionFont& operator=(Menu_MissionFont&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _Delta) override;
private:
	std::shared_ptr<class GameEngineUIRenderer> ExPlaneTex = nullptr;
	float4 Pos = { -480.0f,280.0f,0.0f };
	float4 Scale = {452.0f,154.0f,0.0f};
};
