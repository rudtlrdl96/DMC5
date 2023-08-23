#pragma once

#include "ThirdParty/PhysX_4_1/inc/PhysX/foundation/PxErrorCallback.h"

// Ό³Έν :
class PhysicsLevel
{
public:
	// constrcuter destructer
	PhysicsLevel();
	~PhysicsLevel();

	// delete Function
	PhysicsLevel(const PhysicsLevel& _Other) = delete;
	PhysicsLevel(PhysicsLevel&& _Other) noexcept = delete;
	PhysicsLevel& operator=(const PhysicsLevel& _Other) = delete;
	PhysicsLevel& operator=(PhysicsLevel&& _Other) noexcept = delete;

	physx::PxPhysics* GetLevelPhysics()
	{
		return m_pPhysics;
	}

	physx::PxScene* GetLevelScene()
	{
		return m_pScene;
	}

	bool GetIsPhysicsStop()
	{
		return IsPhysicsStop;
	}

protected:
	class  UserErrorCallback : public  physx::PxErrorCallback
	{
	public:
		void reportError(physx::PxErrorCode::Enum  code, const  char* message, const  char* file, int  line)
		{
			MsgAssert(message);
		}
	};

	void CreatePhysicsX();
	void Simulate(float _DeltaTime);
	void ReleasePhysicsX();

	bool IsPhysicsStop = false;

private:
	physx::PxDefaultAllocator		m_Allocator;
	UserErrorCallback m_ErrorCallback;

	physx::PxPhysics* m_pPhysics = nullptr;
	physx::PxFoundation* m_pFoundation = nullptr;

	physx::PxPvd* m_pPvd = nullptr;
	physx::PxPvdTransport* m_pTransport = nullptr;
	physx::PxScene* m_pScene = nullptr;
	physx::PxDefaultCpuDispatcher* m_pDispatcher = nullptr;
	physx::PxMaterial* m_pMaterial = nullptr;
	physx::PxCooking* m_pCooking = nullptr;

};

