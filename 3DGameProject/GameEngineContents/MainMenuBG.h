#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineUIRenderer.h>

// ���� :
class MainMenuBG : public GameEngineActor
{
public:
	// constrcuter destructer
	MainMenuBG();
	~MainMenuBG();

	// delete Function
	MainMenuBG(const MainMenuBG& _Other) = delete;
	MainMenuBG(MainMenuBG&& _Other) noexcept = delete;
	MainMenuBG& operator=(const MainMenuBG& _Other) = delete;
	MainMenuBG& operator=(MainMenuBG&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
private:
	std::shared_ptr<GameEngineUIRenderer> MainMenuBGRender = nullptr;
	float4 Scale = { 1600.0f,900.0f,0.0f };
};

