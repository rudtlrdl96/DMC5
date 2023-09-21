#pragma once

// Ό³Έν :
class Plane : public GameEngineActor
{
public:
	// constrcuter destructer
	Plane();
	~Plane();

	// delete Function
	Plane(const Plane& _Other) = delete;
	Plane(Plane&& _Other) noexcept = delete;
	Plane& operator=(const Plane& _Other) = delete;
	Plane& operator=(Plane&& _Other) noexcept = delete;

	std::shared_ptr<class PhysXBoxComponent> Component = nullptr;
protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void UserUpdate(float _DeltaTime);
	void ServerUpdate(float _DeltaTime);

private:

};

