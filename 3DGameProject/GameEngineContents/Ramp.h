#pragma once

// Ό³Έν :
class Ramp : public GameEngineActor, public GameEngineNetObject
{
public:
	// constrcuter destructer
	Ramp();
	~Ramp();

	// delete Function
	Ramp(const Ramp& _Other) = delete;
	Ramp(Ramp&& _Other) noexcept = delete;
	Ramp& operator=(const Ramp& _Other) = delete;
	Ramp& operator=(Ramp&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void UserUpdate(float _DeltaTime);
	void ServerUpdate(float _DeltaTime);

private:
	std::shared_ptr<class GameEngineFBXRenderer> Renderer = nullptr;
	std::shared_ptr<class PhysXTriangleComponent> Component = nullptr;

};

