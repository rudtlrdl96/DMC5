#include "PreCompileHeader.h"
#include "PhysicsLevel.h"

#include <PhysXSDKSnippets/SnippetUtils.h>
#include <PhysXSDKSnippets/SnippetPrint.h>
#include <PhysXSDKSnippets/SnippetPVD.h>

PhysicsLevel::PhysicsLevel() 
{
}

PhysicsLevel::~PhysicsLevel() 
{
	Release();
}

void PhysicsLevel::Start()
{
	// Initialize();
}

void PhysicsLevel::Update(float _DeltaTime)
{
	Simulate(true);
}

void PhysicsLevel::LevelChangeStart()
{
	Initialize();
}

void PhysicsLevel::LevelChangeEnd()
{
	Release();
}

physx::PxFilterFlags contactReportFilterShader
(
	physx::PxFilterObjectAttributes attributes0,
	physx::PxFilterData filterData0,
	physx::PxFilterObjectAttributes attributes1,
	physx::PxFilterData filterData1,
	physx::PxPairFlags& pairFlags,
	const void* constantBlock, physx::PxU32 constantBlockSize
)
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
	pairFlags = physx::PxPairFlag::eSOLVE_CONTACT
		| physx::PxPairFlag::eDETECT_DISCRETE_CONTACT
		| physx::PxPairFlag::eNOTIFY_TOUCH_FOUND
		| physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS
		| physx::PxPairFlag::eNOTIFY_TOUCH_LOST
		| physx::PxPairFlag::eNOTIFY_CONTACT_POINTS;

	return physx::PxFilterFlag::eDEFAULT;
}

void PhysicsLevel::Initialize()
{
	m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_ErrorCallback);

	m_pPvd = PxCreatePvd(*m_pFoundation);
	physx::PxPvdTransport* pTransport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	m_pPvd->connect(*pTransport, physx::PxPvdInstrumentationFlag::eALL);

	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, physx::PxTolerancesScale(), true, m_pPvd);

	physx::PxSceneDesc SceneDesc(m_pPhysics->getTolerancesScale());
	SceneDesc.gravity = physx::PxVec3(0.0f, -10.f, 0.0f);
	m_pDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	SceneDesc.cpuDispatcher = m_pDispatcher;
	SceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	m_pScene = m_pPhysics->createScene(SceneDesc);

	physx::PxPvdSceneClient* pPvdClient = m_pScene->getScenePvdClient();
	if (pPvdClient)
	{
		pPvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pPvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pPvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	m_pMaterial = m_pPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	physx::PxRigidStatic* pGroundPlane = PxCreatePlane(*m_pPhysics, physx::PxPlane(0, 1, 0, 0), *m_pMaterial);
	m_pScene->addActor(*pGroundPlane);

	float4 tmpWorldPosition = { pGroundPlane->getGlobalPose().p.x , pGroundPlane->getGlobalPose().p.y  , pGroundPlane->getGlobalPose().p.z };

}

// 실제로 물리연산을 실행
void PhysicsLevel::Simulate(bool _Value)
{
	m_pScene->simulate(1.0f / 60.0f);
	m_pScene->fetchResults(_Value);
}

// 메모리제거
void PhysicsLevel::Release()
{
	PX_RELEASE(m_pScene);
	PX_RELEASE(m_pDispatcher);
	PX_RELEASE(m_pPhysics);
	if (m_pPvd)
	{
		physx::PxPvdTransport* pTransport = m_pPvd->getTransport();
		m_pPvd->release();
		m_pPvd = nullptr;
		PX_RELEASE(pTransport);
	}
	PX_RELEASE(m_pFoundation);

	//if (nullptr != mScene)
	//{
	//	PX_RELEASE(mScene);
	//	mScene = NULL;
	//}
	//if (nullptr != mDispatcher)
	//{
	//	PX_RELEASE(mDispatcher);
	//	mDispatcher = NULL;
	//}
	//if (nullptr != mPhysics)
	//{
	//	PX_RELEASE(mPhysics);
	//	mPhysics = NULL;
	//}
	//if (nullptr != mFoundation)
	//{
	//	PX_RELEASE(mFoundation);
	//	mFoundation = NULL;
	//}
}