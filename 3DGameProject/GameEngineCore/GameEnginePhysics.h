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

	static void Release();
	static HRESULT Initialize();
	static void Render(std::shared_ptr<class GameEngineLevel> Level, float _DeltaTime);
	void Simulate(const float _fDeltaTime);
	void FetchResults(const bool _bBlock);
	HRESULT CreateScene(const UINT _nSceneID);
	HRESULT ChangeScene(const UINT _nSceneID);

	physx::PxPhysics* GetPxPhysics()
	{
		return mPhysics;
	}

	physx::PxMaterial* GetDefaultMaterial()
	{
		return mDefaultMaterial;
	}

	//PxCooking* GetCooking()
	//{
	//	return m_pCooking;
	//}

	//void SetGravity(PxVec3 _vGravity)
	//{
	//	if (nullptr == mScene)
	//	{
	//		return;
	//	}

	//	m_pScene->setGravity(_vGravity);
	//}

	//float4 GetGravity()
	//{
	//	if (nullptr == mScene)
	//	{
	//		return float4(0.f, 0.f, 0.f);
	//	}
	//		
	//	PxVec3 vGravity = mScene->getGravity();
	//	return float4(vGravity.x, vGravity.y, vGravity.z);
	//}

protected:

private:
	static UserErrorCallback gDefaultErrorCallback;
	static physx::PxDefaultAllocator gDefaultAllocatorCallback;

	static physx::PxFoundation* mFoundation;
	static physx::PxPvd* mPVD;
	static physx::PxPhysics* mPhysics;
	static physx::PxMaterial* mDefaultMaterial;
	static physx::PxDefaultCpuDispatcher* mDispatcher;

	physx::PxScene* mScene;
	std::unordered_map<UINT, physx::PxScene*> mapScene;

	//static physx::PxCookingParams* mCooking;
	static physx::PxDefaultCpuDispatcher* InitializeCpuDispatcher(/*physx::PxPhysics* _Physics*/);

};

