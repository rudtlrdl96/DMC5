#pragma once

// Ό³Έν :
class Plane : public GameEngineActor, public GameEngineNetObject
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

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void UserUpdate(float _DeltaTime);
	void ServerUpdate(float _DeltaTime);

private:
	std::shared_ptr<class PhysXBoxGeometryComponent> Component = nullptr;

};

