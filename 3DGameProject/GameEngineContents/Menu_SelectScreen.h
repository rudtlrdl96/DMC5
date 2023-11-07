#pragma once
#include <GameEngineCore/GameEngineActor.h>
// Ό³Έν :
class Menu_SelectScreen :public GameEngineActor
{
public:
	// constrcuter destructer
	Menu_SelectScreen();
	~Menu_SelectScreen();

	// delete Function
	Menu_SelectScreen(const Menu_SelectScreen& _Other) = delete;
	Menu_SelectScreen(Menu_SelectScreen&& _Other) noexcept = delete;
	Menu_SelectScreen& operator=(const Menu_SelectScreen& _Other) = delete;
	Menu_SelectScreen& operator=(Menu_SelectScreen&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _Delta) override;
private:
	void SetFontText();
	void SetButtonIndex();

	std::shared_ptr<class UIButton> MissionButton= nullptr;
	std::shared_ptr<class UIButton> CustomizeButton = nullptr;
	std::shared_ptr<class UIButton> PrevButton = nullptr;
	float4 SelectBaseScale = { 579.0f,50.0f,0.0f };
	float4 SelectBasePos = { -400.0f, 0.0f, 0.0f };
	float4 T_EnterScale = { 416.0f,50.0f,0.0f };
	float4 FontScale = { 0.0f,24.f,0.0f };
	float4 FontPos = { -15.0f,22.f,0.0f };
	float4 ExplanePos = { -650.0f, -320.f, 0.0f };
	int SelectValue = 0;
}; 

