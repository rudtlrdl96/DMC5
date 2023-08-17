#pragma once
#include <GameEngineCore/GameEngineActor.h>
// Ό³Έν :
class BackGround : public GameEngineActor
{
public:
	// constrcuter destructer
	BackGround();
	~BackGround();

	// delete Function
	BackGround(const BackGround& _Other) = delete;
	BackGround(BackGround&& _Other) noexcept = delete;
	BackGround& operator=(const BackGround& _Other) = delete;
	BackGround& operator=(BackGround&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
private:
	std::shared_ptr<class GameEngineSpriteRenderer> BGRender = nullptr;
	float4 Pos = { 0.0f,0.0f,0.0f };
};

