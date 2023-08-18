#pragma once

#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineCore/GameEngineUIRenderer.h>

// Ό³Έν :
class StartButton : public GameEngineActor
{
public:
	// constrcuter destructer
	StartButton();
	~StartButton();

	// delete Function
	StartButton(const StartButton& _Other) = delete;
	StartButton(StartButton&& _Other) noexcept = delete;
	StartButton& operator=(const StartButton& _Other) = delete;
	StartButton& operator=(StartButton&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
private:
	void Blink(float _DeltaTime);
	std::shared_ptr<GameEngineUIRenderer> StartButtonPtr = nullptr;
	std::shared_ptr< GameEngineCollision> ButtonCol = nullptr;
	float4 Pos = { 0.0f,-200.0f,0.0f };
	float4 Scale = { 200.0f,50.0f,1.0f };
	float Time = 0.0f;
	bool SwichValue = false;
};

