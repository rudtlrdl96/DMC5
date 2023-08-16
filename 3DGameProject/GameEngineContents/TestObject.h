#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineBase/GameEngineNetObject.h>

// ���� :
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
	std::shared_ptr<class PhysXBoxGeometryComponent> Component = nullptr;

};

