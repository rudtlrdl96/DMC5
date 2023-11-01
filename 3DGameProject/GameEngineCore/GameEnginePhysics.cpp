#include "PreCompileHeader.h"
#include "GameEnginePhysics.h"

#include <PhysXSDKSnippets/SnippetUtils.h>
#include <PhysXSDKSnippets/SnippetPrint.h>
#include <PhysXSDKSnippets/SnippetPVD.h>

#include "GameEngineLevel.h"

physx::PxDefaultAllocator GameEnginePhysics::m_Allocator;
UserErrorCallback GameEnginePhysics::m_ErrorCallback;

physx::PxPhysics* GameEnginePhysics::m_pPhysics = nullptr;
physx::PxFoundation* GameEnginePhysics::m_pFoundation = nullptr;

physx::PxPvd* GameEnginePhysics::m_pPvd = nullptr;
physx::PxPvdTransport* GameEnginePhysics::m_pTransport = nullptr;
physx::PxScene* GameEnginePhysics::m_pScene = nullptr;
physx::PxDefaultCpuDispatcher* GameEnginePhysics::m_pDispatcher = nullptr;
physx::PxMaterial* GameEnginePhysics::m_pMaterial = nullptr;
physx::PxCooking* GameEnginePhysics::m_pCooking = nullptr;

CustomCallback* GameEnginePhysics::CustomCallbackEvent;
std::map<std::string, physx::PxScene*> GameEnginePhysics::AllScene;

float GameEnginePhysics::SceneGravity = 0.0f;
bool GameEnginePhysics::IsPhysics = false;

GameEnginePhysics::GameEnginePhysics() 
{
}

GameEnginePhysics::~GameEnginePhysics() 
{
}

physx::PxFilterFlags CustomFilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0, physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1, physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
{
	// let triggers through
	if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT | physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS;

		return physx::PxFilterFlag::eDEFAULT;
	}

	//generate contacts for all that were not filtered above
	pairFlags = 
		physx::PxPairFlag::eNOTIFY_TOUCH_FOUND | 
		physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS | 
		physx::PxPairFlag::eNOTIFY_TOUCH_LOST |
		physx::PxPairFlag::eDETECT_DISCRETE_CONTACT | 
		physx::PxPairFlag::eSOLVE_CONTACT;

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

void GameEnginePhysics::CreatePhysicsX()
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

	m_pDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	if (!m_pDispatcher)
	{
		MsgAssert("Dispatcher 생성 실패");
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
	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, physx::PxTolerancesScale(), true, m_pPvd);
	if (!m_pPhysics)
	{
		MsgAssert("PxPhysics 생성 실패");
	}

	m_pCooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_pFoundation, physx::PxCookingParams(m_pPhysics->getTolerancesScale()));
	if (!m_pCooking)
	{
		MsgAssert("PxCooking 생성 실패");
	}

	// Aggregate생성
	//MapAggregate_ = Physics_->createAggregate(2000, false);
	//Scene_->addAggregate(*MapAggregate_);
}

void GameEnginePhysics::CreateScene(const std::string_view& _Name)
{
	std::string Name = GameEngineString::ToUpper(_Name);

	if (AllScene.end() != AllScene.find(Name))
	{
		MsgAssert("같은 이름의 씬이 존재합니다." + Name);
	}

	physx::PxSceneDesc SceneDesc(m_pPhysics->getTolerancesScale());
	physx::PxScene* NewScene = nullptr;
	SceneGravity = 2000.0f;
	CustomCallbackEvent = new CustomCallback();

	SceneDesc.gravity = physx::PxVec3(0.0f, -SceneGravity, 0.0f);
	SceneDesc.cpuDispatcher = m_pDispatcher;
	SceneDesc.simulationEventCallback = CustomCallbackEvent;
	SceneDesc.filterShader = CustomFilterShader;

	NewScene = m_pPhysics->createScene(SceneDesc);
	if (!NewScene)
	{
		MsgAssert("PxScene 생성 실패");
	}

	physx::PxPvdSceneClient* pPvdClient = NewScene->getScenePvdClient();
	if (pPvdClient)
	{
		pPvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pPvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pPvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	AllScene.emplace(Name, NewScene);
}

void GameEnginePhysics::ChangeScene(const std::string_view& _Name)
{
	std::string Name = GameEngineString::ToUpper(_Name);

	if (AllScene.end() == AllScene.find(Name))
	{
		return;
	}
	
	m_pScene = AllScene[Name];
}

// 실제로 물리연산을 실행
void GameEnginePhysics::Simulate(float _DeltaTime)
{
	if (nullptr == m_pPhysics ||
		nullptr == m_pScene ||
		true == IsPhysics ||
		0.0f >= _DeltaTime)
	{
		return;
	}

	m_pScene->simulate(_DeltaTime);
	m_pScene->fetchResults(true);
}

bool GameEnginePhysics::RayCast(const float4& _vOrigin, const float4& _vDir, OUT float4& _vPoint, float _fDistance)
{
	physx::PxVec3 vOrigin(0.f, 0.f, 0.f);

	memcpy_s(&vOrigin, sizeof(physx::PxVec3), &_vOrigin, sizeof(physx::PxVec3));

	physx::PxVec3 vDir(0.f, 0.f, 0.f);

	float4 vDir_ = _vDir;
	float4 vNormalizedDir = vDir_.NormalizeReturn();

	memcpy_s(&vDir, sizeof(physx::PxVec3), &vNormalizedDir, sizeof(physx::PxVec3));

	physx::PxRaycastBuffer tRayCastBuff;

	if (true == m_pScene->raycast(vOrigin, vDir, (physx::PxReal)_fDistance, tRayCastBuff))
	{
		if (true == tRayCastBuff.hasBlock)
		{
			physx::PxRaycastHit tRayCastHit = tRayCastBuff.block;
			memcpy_s(&_vPoint, sizeof(float4), &tRayCastHit.position, sizeof(float4));

			return true;
		}

		return false;
	}

	return false;
}

// 메모리제거
void GameEnginePhysics::ReleasePhysicsX()
{
	if (nullptr != m_pCooking)
	{
		PX_RELEASE(m_pCooking);
	}
	{
		std::map<std::string, physx::PxScene*>::iterator ReleaseStartIter = AllScene.begin();
		std::map<std::string, physx::PxScene*>::iterator ReleaseEndIter = AllScene.end();

		while (ReleaseStartIter != ReleaseEndIter)
		{
			physx::PxScene* ReleaseScene = ReleaseStartIter->second;
			delete ReleaseScene->getSimulationEventCallback();
			PX_RELEASE(ReleaseScene);
			++ReleaseStartIter;
		}
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