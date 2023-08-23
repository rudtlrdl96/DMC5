#pragma once

// Ό³Έν :
class Ball : public GameEngineActor, public GameEngineNetObject
{
public:
	// constrcuter destructer
	Ball();
	~Ball();

	// delete Function
	Ball(const Ball& _Other) = delete;
	Ball(Ball&& _Other) noexcept = delete;
	Ball& operator=(const Ball& _Other) = delete;
	Ball& operator=(Ball&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void UserUpdate(float _DeltaTime);
	void ServerUpdate(float _DeltaTime);

private:
	std::shared_ptr<class GameEngineFBXRenderer> Renderer = nullptr;
	std::shared_ptr<class PhysXSphereComponent> Component = nullptr;

};

