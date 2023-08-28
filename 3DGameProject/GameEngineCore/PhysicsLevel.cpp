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
}

//physx::PxFilterFlags contactReportFilterShader
//(
//	physx::PxFilterObjectAttributes attributes0,
//	physx::PxFilterData filterData0,
//	physx::PxFilterObjectAttributes attributes1,
//	physx::PxFilterData filterData1,
//	physx::PxPairFlags& pairFlags,
//	const void* constantBlock, physx::PxU32 constantBlockSize
//)
//{
//	PX_UNUSED(attributes0);
//	PX_UNUSED(attributes1);
//	PX_UNUSED(filterData0);
//	PX_UNUSED(filterData1);
//	PX_UNUSED(constantBlockSize);
//	PX_UNUSED(constantBlock);
//
//	if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
//	{
//		pairFlags = physx::PxPairFlag::eNOTIFY_TOUCH_FOUND | physx::PxPairFlag::eNOTIFY_TOUCH_LOST | physx::PxPairFlag::eSOLVE_CONTACT | physx::PxPairFlag::eDETECT_DISCRETE_CONTACT;
//		return physx::PxFilterFlag::eDEFAULT;
//	}
//
//	// all initial and persisting reports for everything, with per-point data
//	pairFlags = physx::PxPairFlag::eSOLVE_CONTACT
//		| physx::PxPairFlag::eDETECT_DISCRETE_CONTACT
//		| physx::PxPairFlag::eNOTIFY_TOUCH_FOUND
//		| physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS
//		| physx::PxPairFlag::eNOTIFY_TOUCH_LOST
//		| physx::PxPairFlag::eNOTIFY_CONTACT_POINTS;
//
//	return physx::PxFilterFlag::eDEFAULT;
//}

physx::PxFilterFlags CustomFilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0, physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1, physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
{
	// SampleSubmarineFilterShader로부터 가져옴
	// 
	// let triggers through
	if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT | physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
		return physx::PxFilterFlag::eDEFAULT;
	}
	//generate contacts for all that were not filtered above
	pairFlags = physx::PxPairFlag::eNOTIFY_TOUCH_FOUND | physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS | physx::PxPairFlag::eNOTIFY_TOUCH_LOST |
		physx::PxPairFlag::eDETECT_DISCRETE_CONTACT | physx::PxPairFlag::eSOLVE_CONTACT;

	return physx::PxFilterFlag::eDEFAULT;

	//if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
	//{
	//	pairFlags |= physx::PxPairFlag::eNOTIFY_CONTACT_POINTS;
	//	pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;

	//}

	//if ((filterData0.word0 & filterData1.word2) && (filterData1.word0 & filterData0.word2))
	//{
	//	pairFlags |= physx::PxPairFlag::eNOTIFY_CONTACT_POINTS;
	//	pairFlags |= physx::PxPairFlag::eMODIFY_CONTACTS;
	//}
}


void PhysicsLevel::CreatePhysicsX()
{
	if (nullptr != m_pPhysics)
	{
		return;
	}

	m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_ErrorCallback);
	if (!m_pFoundation)
	{
		MsgAssert("PxFoundation 생성 실패");
	}

#ifdef _DEBUG
	m_pPvd = PxCreatePvd(*m_pFoundation);
	if (!m_pPvd)
	{
		MsgAssert("PxPvd 생성 실패");
	}

	m_pTransport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	if (!m_pTransport)
	{
		MsgAssert("Transport 생성 실패");
	}

	m_pPvd->connect(*m_pTransport, physx::PxPvdInstrumentationFlag::eALL);

#endif
	physx::PxTolerancesScale TolerancesScale;

	TolerancesScale.length = 1.f;
	TolerancesScale.speed = 10.f;

	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, TolerancesScale, true, m_pPvd);
	if (!m_pPhysics)
	{
		MsgAssert("PxPhysics 생성 실패");
	}

	physx::PxSceneDesc SceneDesc(m_pPhysics->getTolerancesScale());

	SceneDesc.gravity = physx::PxVec3(0.0f, -400.0f, 0.0f);
	m_pDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	SceneDesc.cpuDispatcher = m_pDispatcher;
	// EventCallback 세팅
	CustomCallbackEvent = new CustomCallback();
	SceneDesc.simulationEventCallback = CustomCallbackEvent;
	// callback을 호출 처리할 filtershader 세팅
	SceneDesc.filterShader = CustomFilterShader;

	m_pScene = m_pPhysics->createScene(SceneDesc);
	if (!m_pScene)
	{
		MsgAssert("PxScene 생성 실패");
	}

	SceneDesc.filterShader = CustomFilterShader;

	physx::PxPvdSceneClient* pPvdClient = m_pScene->getScenePvdClient();
	if (pPvdClient)
	{
		pPvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pPvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pPvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	m_pCooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_pFoundation, physx::PxCookingParams(m_pPhysics->getTolerancesScale()));
	if (!m_pCooking)
	{
		MsgAssert("PxCooking 생성 실패");
	}

	StepSize = 1.0f / 60.0f;

	// Aggregate생성
	//MapAggregate_ = Physics_->createAggregate(2000, false);
	//Scene_->addAggregate(*MapAggregate_);
}

void PhysicsLevel::advance(physx::PxReal _DeltaTime)
{
	WaitTime += _DeltaTime;
	StepSize = 1.0f / 60.0f;

	if (WaitTime < StepSize)
	{
		IsSimulation = false;
	}
	else
	{
		WaitTime -= StepSize;

		m_pScene->simulate(StepSize);

		IsSimulation = true;
	}
}

// 실제로 물리연산을 실행
void PhysicsLevel::Simulate(float _DeltaTime)
{
	if (nullptr == m_pPhysics)
	{
		return;
	}

	if (true == IsPhysics)
	{
		return;
	}

	advance(_DeltaTime);

	if (true == IsSimulation)
	{
		m_pScene->fetchResults(true);
	}
}

// 메모리제거
void PhysicsLevel::ReleasePhysicsX()
{
	if (nullptr != CustomCallbackEvent)
	{
		delete CustomCallbackEvent;
		CustomCallbackEvent = nullptr;
	}
	if (nullptr != m_pCooking)
	{
		PX_RELEASE(m_pCooking);
	}
	if (nullptr != m_pScene)
	{
		PX_RELEASE(m_pScene);
	}
	if (nullptr != m_pDispatcher)
	{
		PX_RELEASE(m_pDispatcher);
	}
	if (nullptr != m_pPhysics)
	{
		PX_RELEASE(m_pPhysics);
	}
	if (nullptr != m_pPvd)
	{
		PX_RELEASE(m_pPvd);
	}
	if (nullptr != m_pTransport)
	{
		PX_RELEASE(m_pTransport);
	}
	if (nullptr != m_pFoundation)
	{
		PX_RELEASE(m_pFoundation);
	}
}