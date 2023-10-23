#pragma once
#include <GameEngineCore/GameEngineActor.h>
// 설명 :
class Intro_Logo : public GameEngineActor
{
public:
	// constrcuter destructer
	Intro_Logo();
	~Intro_Logo();

	// delete Function
	Intro_Logo(const Intro_Logo& _Other) = delete;
	Intro_Logo(Intro_Logo&& _Other) noexcept = delete;
	Intro_Logo& operator=(const Intro_Logo& _Other) = delete;
	Intro_Logo& operator=(Intro_Logo&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
private:

	std::shared_ptr<class GameEngineSpriteRenderer> CapComIntro_Logo = nullptr;
	std::shared_ptr<class GameEngineSpriteRenderer> PhysxIntro_Logo = nullptr;
	//변수
	bool Value = false;
	float Time = 0.0f;
	float Ratio = 0.5f;
	float4 Pos = { 0.0f,0.0f,-1.0f };
	float4 Scale = { 640.0f,320.0f,0.0f };
};

