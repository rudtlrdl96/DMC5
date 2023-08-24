#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineBase/GameEngineNetObject.h>

// Ό³Έν :
class TestObject : public GameEngineActor, public GameEngineNetObject
{
public:
	// constrcuter destructer
	TestObject();
	~TestObject();

	// delete Function
	TestObject(const TestObject& _Other) = delete;
	TestObject(TestObject&& _Other) noexcept = delete;
	TestObject& operator=(const TestObject& _Other) = delete;
	TestObject& operator=(TestObject&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void UserUpdate(float _DeltaTime);
	void ServerUpdate(float _DeltaTime);

private:
	//std::shared_ptr<class PhysXBoxComponent> Component = nullptr;
	std::shared_ptr<class GameEngineFBXRenderer> Renderer = nullptr;
	std::shared_ptr<class PhysXCapsuleComponent> Component = nullptr;
	float4 Velocity = float4::ZERO;
	float4 Rot = float4::ZERO;
};

