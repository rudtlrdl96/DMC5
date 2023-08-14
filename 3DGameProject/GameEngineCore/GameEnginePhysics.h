#pragma once

#include "GameEngineLevel.h"
#include "ThirdParty/PhysX_5/inc/PxPhysicsAPI.h"
#include "ThirdParty/PhysX_5/inc/PxConfig.h"
#include "ThirdParty/PhysX_5/inc/foundation/PxErrorCallback.h"

#pragma comment(lib, "PhysX_64.lib")
#pragma comment(lib, "PhysXCommon_64.lib")
#pragma comment(lib, "PhysXFoundation_64.lib")

#pragma comment(lib, "PhysXFoundation_64.lib")
#pragma comment(lib, "PhysXFoundation_64.lib")

#pragma comment(lib, "PhysXExtensions_static_64.lib")
#pragma comment(lib, "PhysXCooking_64.lib")
#pragma comment(lib, "PhysXCharacterKinematic_static_64.lib")

//#pragma comment (lib, "PhysX_static_64.lib")
//#pragma comment (lib, "PhysXCommon_static_64.lib")
//#pragma comment (lib, "PhysXFoundation_static_64.lib")
//#pragma comment (lib, "PhysXPvdSDK_static_64.lib")
//#pragma comment (lib, "PhysXCooking_static_64.lib")
//#pragma comment (lib, "PhysXCharacterKinematic_static_64.lib")

// Ό³Έν :
class GameEnginePhysics : public GameEngineLevel
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
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;

	void LevelChangeStart() override;
	void LevelChangeEnd() override;

private:
	//physx::PxDefaultAllocator		DefaultAllocator_;
	UserErrorCallback gDefaultErrorCallback;
	physx::PxDefaultAllocator gDefaultAllocatorCallback;
	physx::PxCookingParams* mCooking;

	static physx::PxPhysics* mPhysics;
	static physx::PxScene* mScene;
	physx::PxFoundation* mFoundation = NULL;
	physx::PxControllerManager* CtrManager_ = NULL;

	physx::PxPvd* mPVD = NULL;
	physx::PxDefaultCpuDispatcher* mDispatcher = NULL;

	physx::PxMaterial* mDefaultMaterial;

	//std::unordered_map<UINT, physx::PxScene*> mapScene;

	physx::PxDefaultCpuDispatcher* InitializeCpuDispatcher(/*physx::PxPhysics* _Physics*/);
	void Initialize();
	void Release();
};

