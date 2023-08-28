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
	//float4 Color = { 0.423f, 0.705f, 0.725f };
};

