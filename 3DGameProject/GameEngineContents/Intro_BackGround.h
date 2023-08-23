#pragma once
#include <GameEngineCore/GameEngineActor.h>
// ���� :
class Intro_BackGround : public GameEngineActor
{
public:
	// constrcuter destructer
	Intro_BackGround();
	~Intro_BackGround();

	// delete Function
	Intro_BackGround(const Intro_BackGround& _Other) = delete;
	Intro_BackGround(Intro_BackGround&& _Other) noexcept = delete;
	Intro_BackGround& operator=(const Intro_BackGround& _Other) = delete;
	Intro_BackGround& operator=(Intro_BackGround&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
private:
	std::shared_ptr<class GameEngineSpriteRenderer> BGRender = nullptr;
	float4 Pos = { 0.0f,0.0f,0.0f };
};

