#include "PrecompileHeader.h"
#include "GameEnginePhysics.h"

#ifdef _DEBUG
#define PVD_HOST "127.0.0.1"
#endif // _DEBUG

physx::PxDefaultAllocator GameEnginePhysics::gDefaultAllocatorCallback;
GameEnginePhysics::UserErrorCallback GameEnginePhysics::gDefaultErrorCallback;

physx::PxFoundation* GameEnginePhysics::mFoundation = nullptr;
physx::PxPhysics* GameEnginePhysics::mPhysics = nullptr;
physx::PxMaterial* GameEnginePhysics::mDefaultMaterial = nullptr;
physx::PxPvd* GameEnginePhysics::mPVD = nullptr;
physx::PxDefaultCpuDispatcher* GameEnginePhysics::mDispatcher = nullptr;

GameEnginePhysics::GameEnginePhysics() 
{
}

GameEnginePhysics::~GameEnginePhysics() 
{
}

physx::PxFilterFlags contactReportFilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
	physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
	physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
{
	PX_UNUSED(attributes0);
	PX_UNUSED(attributes1);
	PX_UNUSED(filterData0);
	PX_UNUSED(filterData1);
	PX_UNUSED(constantBlockSize);
	PX_UNUSED(constantBlock);

	if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = physx::PxPairFlag::eNOTIFY_TOUCH_FOUND | physx::PxPairFlag::eNOTIFY_TOUCH_LOST | physx::PxPairFlag::eSOLVE_CONTACT | physx::PxPairFlag::eDETECT_DISCRETE_CONTACT;
		return physx::PxFilterFlag::eDEFAULT;
	}

	// all initial and persisting reports for everything, with per-point data
	pairFlags = physx::PxPairFlag::eSOLVE_CONTACT | physx::PxPairFlag::eDETECT_DISCRETE_CONTACT
		| physx::PxPairFlag::eNOTIFY_TOUCH_FOUND
		| physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS
		| physx::PxPairFlag::eNOTIFY_TOUCH_LOST
		| physx::PxPairFlag::eNOTIFY_CONTACT_POINTS;

	return physx::PxFilterFlag::eDEFAULT;
}

HRESULT GameEnginePhysics::Initialize()
{
	mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
	
	if (nullptr == mFoundation)
	{
		MsgAssert("PxCreateFoundation 실패");
	}

#ifdef _DEBUG
	//mPVD = physx::PxCreatePvd(*mFoundation);
	//if (nullptr == mPVD)
	//{
	//	MsgAssert("PxCreatePvd 실패");
	//	return E_FAIL;
	//}
	//physx::PxPvdTransport* pPVDTransport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	//mPVD->connect(*pPVDTransport, physx::PxPvdInstrumentationFlag::eALL);
#endif

	bool recordMemoryAllocations = true;
	physx::PxTolerancesScale Tolerancesscale = physx::PxTolerancesScale();
	Tolerancesscale.length = 1;
	Tolerancesscale.speed = 10;
		
	mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, Tolerancesscale, recordMemoryAllocations);
	
	if (nullptr == mPhysics)
	{
		MsgAssert("PxCreatePhysics 실패");
	}

	mDefaultMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.f);

	// mDispatcher = physx::PxDefaultCpuDispatcherCreate(2);

	//physx::PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
	//sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);

	//mScene = mPhysics->createScene(sceneDesc);

	return S_OK;
}

void GameEnginePhysics::Render(std::shared_ptr<class GameEngineLevel> Level, float _DeltaTime)
{
	//physx::PxRigidDynamic* box = PxCreateDynamic(*physics, physx::PxTransform(physx::PxVec3(0, 10, 0)), physx::PxBoxGeometry(1, 1, 1), *material, 10.0f);

	//scene->addActor(*box);
	//scene->simulate(_DeltaTime);
	//scene->fetchResults(true);
}

void GameEnginePhysics::Release()
{
	//scene->release();
	if (nullptr != mPhysics)
	{
		PX_RELEASE(mPhysics);
	}
	if (nullptr != mFoundation)
	{
		PX_RELEASE(mFoundation);
	}

}