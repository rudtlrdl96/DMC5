#pragma once

// Ό³Έν :
class Box300 : public GameEngineActor, public GameEngineNetObject
{
public:
	// constrcuter destructer
	Box300();
	~Box300();

	// delete Function
	Box300(const Box300& _Other) = delete;
	Box300(Box300&& _Other) noexcept = delete;
	Box300& operator=(const Box300& _Other) = delete;
	Box300& operator=(Box300&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void UserUpdate(float _DeltaTime);
	void ServerUpdate(float _DeltaTime);

private:
	std::shared_ptr<class GameEngineFBXRenderer> Renderer = nullptr;
	std::shared_ptr<class PhysXBoxComponent> Component = nullptr;

};

