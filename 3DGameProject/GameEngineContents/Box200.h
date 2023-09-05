#pragma once

// Ό³Έν :
class Box200 : public GameEngineActor, public GameEngineNetObject, public PhysicsActor
{
public:
	// constrcuter destructer
	Box200();
	~Box200();

	// delete Function
	Box200(const Box200& _Other) = delete;
	Box200(Box200&& _Other) noexcept = delete;
	Box200& operator=(const Box200& _Other) = delete;
	Box200& operator=(Box200&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void UserUpdate(float _DeltaTime);
	void ServerUpdate(float _DeltaTime);

private:
	std::shared_ptr<class GameEngineFBXRenderer> Renderer = nullptr;
	std::shared_ptr<class PhysXBoxComponent> Component = nullptr;

};

