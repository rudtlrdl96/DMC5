#pragma once

//#include <GameEngineCore/ThirdParty/PhysX 5/inc/extensions/PxDefaultAllocator.h>
//#include <GameEngineCore/ThirdParty/PhysX 5/inc/extensions/PxDefaultErrorCallback.h>
//#include <GameEngineCore/ThirdParty/PhysX 5/inc/foundation/PxFoundation.h>
//#include <GameEngineCore/ThirdParty/PhysX 5/inc/PxPhysics.h>
//#include <GameEngineCore/ThirdParty/PhysX 5/inc/PxMaterial.h>

// Ό³Έν :
class TestLevel : public GameEngineLevel
{
public:
	// constrcuter destructer
	TestLevel();
	~TestLevel();

	// delete Function
	TestLevel(const TestLevel& _Other) = delete;
	TestLevel(TestLevel&& _Other) noexcept = delete;
	TestLevel& operator=(const TestLevel& _Other) = delete;
	TestLevel& operator=(TestLevel&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render() {};

	void LevelChangeStart() override;
	void LevelChangeEnd() override;

private:
	bool IsMessage = false;

	//physx::PxDefaultAllocator gAllocator;
	//physx::PxDefaultErrorCallback gErrorCallback;
	//physx::PxFoundation* foundation = nullptr;
	//physx::PxPhysics* physics = nullptr;
	//physx::PxMaterial* gMaterial = nullptr;

	int PhysXFunction();
};

