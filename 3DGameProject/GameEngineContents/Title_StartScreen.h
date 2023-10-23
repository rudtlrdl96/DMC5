#pragma once

#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineCore/GameEngineUIRenderer.h>

// Ό³Έν :
class Title_StartScreen : public GameEngineActor
{
public:
	// constrcuter destructer
	Title_StartScreen();
	~Title_StartScreen();

	// delete Function
	Title_StartScreen(const Title_StartScreen& _Other) = delete;
	Title_StartScreen(Title_StartScreen&& _Other) noexcept = delete;
	Title_StartScreen& operator=(const Title_StartScreen& _Other) = delete;
	Title_StartScreen& operator=(Title_StartScreen&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
private:
	void Blink(float _DeltaTime);
	std::shared_ptr<class GameEngineFontRenderer> SelectText = nullptr;
	std::shared_ptr< GameEngineCollision> ButtonCol = nullptr;
	float4 Pos = { 0.0f,-250.0f,0.0f };
	float4 Scale = { 200.0f,50.0f,1.0f };
	float Time = 0.0f;
	bool SwichValue = false;
	float SkyColor_Alpha = 1.0f;
	float4 SkyColor = {float4::ZERONULL};
};

