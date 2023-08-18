#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineUIRenderer.h>

// Ό³Έν :
class MouseCusor : public GameEngineActor
{
public:
	// constrcuter destructer
	MouseCusor();
	~MouseCusor();

	// delete Function
	MouseCusor(const MouseCusor& _Other) = delete;
	MouseCusor(MouseCusor&& _Other) noexcept = delete;
	MouseCusor& operator=(const MouseCusor& _Other) = delete;
	MouseCusor& operator=(MouseCusor&& _Other) noexcept = delete;
protected:

	void Start() override;
	void Update(float _DeltaTime) override;
private:
	float4 MouseCusorScale = { 76.0f,76.0f,0.0f };
	std::shared_ptr<class GameEngineUIRenderer> MouseCusorRender;
	std::shared_ptr<class GameEngineCollision> MouseCusorCollsion;

};

