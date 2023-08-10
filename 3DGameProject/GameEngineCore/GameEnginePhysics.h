#pragma once

#include "ThirdParty/PhysX_5/inc/PxPhysicsAPI.h"
#include "ThirdParty/PhysX_5/inc/foundation/PxErrorCallback.h"

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

	class UserErrorCallback : public physx::PxErrorCallback
	{
	public:
		void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) override
		{
			MsgAssert(message);
		}
	};

	static HRESULT Initialize();
	static void Render(std::shared_ptr<class GameEngineLevel> Level, float _DeltaTime);
	static void Release();

protected:

private:
	static UserErrorCallback gDefaultErrorCallback;
	static physx::PxDefaultAllocator gDefaultAllocatorCallback;

	static physx::PxFoundation* mFoundation;
	static physx::PxPvd* mPVD;
	static physx::PxPhysics* mPhysics;
	static physx::PxMaterial* mDefaultMaterial;
	static physx::PxDefaultCpuDispatcher* mDispatcher;
	//static physx::PxCookingParams* mCooking;

};

