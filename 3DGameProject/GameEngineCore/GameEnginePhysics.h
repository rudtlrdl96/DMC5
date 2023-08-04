#pragma once

#include "ThirdParty/PhysX_5/inc/PxPhysicsAPI.h"
#include "ThirdParty/PhysX_5/inc/extensions/PxDefaultAllocator.h"
#include "ThirdParty/PhysX_5/inc/extensions/PxDefaultErrorCallback.h"

#ifdef _DEBUG
#pragma comment(lib, "PhysX_64.lib")
#pragma comment(lib, "PhysXCommon_64.lib")
#pragma comment(lib, "PhysXFoundation_64.lib")
#pragma comment(lib, "PhysXTask_static_64.lib")
#pragma comment(lib, "PhysXPvdSDK_static_64.lib")
#else
//#pragma comment(lib, "..\\GameEnginePlatform\\ThirdParty\\FMOD\\lib\\x64\\fmod_vc.lib")
#endif


// Ό³Έν :
class GameEnginePhysics
{
public:
	// constrcuter destructer
	GameEnginePhysics();
	~GameEnginePhysics();

	// delete Function
	GameEnginePhysics(const GameEnginePhysics& _Other) = delete;
	GameEnginePhysics(GameEnginePhysics&& _Other) noexcept = delete;
	GameEnginePhysics& operator=(const GameEnginePhysics& _Other) = delete;
	GameEnginePhysics& operator=(GameEnginePhysics&& _Other) noexcept = delete;

	static void Initialize();
	static void Render(std::shared_ptr<class GameEngineLevel> Level, float _DeltaTime);
	static void Release();

protected:

private:
	static physx::PxDefaultAllocator gAllocator;
	static physx::PxDefaultErrorCallback gErrorCallback;

	static physx::PxFoundation* foundation;
	static physx::PxPhysics* physics;
	static physx::PxMaterial* material;
	static physx::PxScene* scene;

};

