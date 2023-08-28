#pragma once

// Ό³Έν :
class Slope : public GameEngineActor, public GameEngineNetObject
{
public:
	// constrcuter destructer
	Slope();
	~Slope();

	// delete Function
	Slope(const Slope& _Other) = delete;
	Slope(Slope&& _Other) noexcept = delete;
	Slope& operator=(const Slope& _Other) = delete;
	Slope& operator=(Slope&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void UserUpdate(float _DeltaTime);
	void ServerUpdate(float _DeltaTime);

private:
	std::shared_ptr<class PhysXBoxComponent> Component = nullptr;

};

