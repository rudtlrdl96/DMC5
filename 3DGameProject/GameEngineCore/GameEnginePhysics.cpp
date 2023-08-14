#include "PrecompileHeader.h"
#include "GameEnginePhysics.h"

#include "ThirdParty/PhysX_5/inc/PxPhysicsAPI.h"
#include "ThirdParty/PhysX_5/inc/PxConfig.h"
#include "ThirdParty/PhysX_5/inc/foundation/PxErrorCallback.h"
#include "ThirdParty/PhysX_5/inc/extensions/PxDefaultCpuDispatcher.h"

#ifdef _DEBUG
#define PVD_HOST "127.0.0.1"
#endif // _DEBUG

physx::PxPhysics* GameEnginePhysics::mPhysics = NULL;
physx::PxScene* GameEnginePhysics::mScene = NULL;

GameEnginePhysics::GameEnginePhysics() 
{
}

GameEnginePhysics::~GameEnginePhysics() 
{
}

void GameEnginePhysics::Start() {}
void GameEnginePhysics::Update(float _DeltaTime) {}
void GameEnginePhysics::Render(float _DeltaTime) {}

void GameEnginePhysics::LevelChangeStart()
{
	Initialize();
}

void GameEnginePhysics::LevelChangeEnd()
{
	Release();
}

void GameEnginePhysics::Release()
{
	//if (nullptr != mCooking)
	//{
	//	PX_RELEASE(mCooking);
	//}
	if (nullptr != mScene)
	{
		PX_RELEASE(mScene);
		mScene = NULL;
	}
	if (nullptr != mDispatcher)
	{
		PX_RELEASE(mDispatcher);
		mDispatcher = NULL;
	}
	if (nullptr != mPhysics)
	{
		PX_RELEASE(mPhysics);
		mPhysics = NULL;
	}
	if (nullptr != mFoundation)
	{
		PX_RELEASE(mFoundation);
		mFoundation = NULL;
	}
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

physx::PxDefaultCpuDispatcher* GameEnginePhysics::InitializeCpuDispatcher(/*physx::PxPhysics* _Physics*/)
{
	const int numThreads = 4; // 원하는 쓰레드 개수
	physx::PxDefaultCpuDispatcher* cpuDispatcher = nullptr;
	//physx::PxDefaultCpuDispatcher* cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(numThreads);
	return cpuDispatcher;
}

void GameEnginePhysics::Initialize()
{
	mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
	if (nullptr == mFoundation)
	{
		MsgAssert("PxCreateFoundation 실패");
	}

#ifdef _DEBUG
	//mPVD = PxCreatePvd(*mFoundation);
	//if (nullptr == mPVD)
	//{
	//	MsgAssert("PxCreatePvd 실패");
	//	return;
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
	//mDispatcher = InitializeCpuDispatcher();
}

void GameEnginePhysics::Render(std::shared_ptr<class GameEngineLevel> Level, float _DeltaTime)
{
	//physx::PxRigidDynamic* box = PxCreateDynamic(*physics, physx::PxTransform(physx::PxVec3(0, 10, 0)), physx::PxBoxGeometry(1, 1, 1), *material, 10.0f);

	//scene->addActor(*box);
	//scene->simulate(_DeltaTime);
	//scene->fetchResults(true);
}

void GameEnginePhysics::Simulate(const float _fDeltaTime)
{
	//if (true == m_bSimulate || nullptr == m_pScene)
	//	return;

	////Simulation 시작 전 변경된 Transform을 Actor에 적용
	//PxU32		nNumActors = 0;
	//PxActor** ppActors = nullptr;

	////Scene에 있는 RigidDynamicActor의 숫자
	//nNumActors = m_pScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC);
	////Scene에 있는 RigidDynamicActor 포인터를 받아오기 위한 동적 배열 생성
	//ppActors = new PxActor * [nNumActors];
	////Scene에서 RigidDynamicActor들을 가져옴.
	//m_pScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC, ppActors, nNumActors);
	////
	//for (UINT i = 0; i < nNumActors; ++i)
	//{
	//	if (ppActors[i]->userData)
	//		((LPPXUSERDATA)(ppActors[i]->userData))->pCollider.lock()->ReadySimulate();
	//}
	////생성한 동적 배열 삭제
	//delete[] ppActors;

	////Scene에 있는 RigidStaticActor의 숫자
	//nNumActors = m_pScene->getNbActors(PxActorTypeFlag::eRIGID_STATIC);
	////Scene에 있는 RigidStaticActor 포인터를 받아오기 위한 동적 배열 생성
	//ppActors = new PxActor * [nNumActors];
	////Scene에서 RigidStaticActor들을 가져옴.
	//m_pScene->getActors(PxActorTypeFlag::eRIGID_STATIC, ppActors, nNumActors);
	////
	//for (UINT i = 0; i < nNumActors; ++i)
	//{
	//	if (ppActors[i]->userData)
	//		((LPPXUSERDATA)(ppActors[i]->userData))->pCollider.lock()->ReadySimulate();
	//}
	////생성한 동적 배열 삭제
	//delete[] ppActors;

	////Simulation 시작
	//m_pScene->simulate(1.f / 60.f);

	//m_bSimulate = true;
}

void GameEnginePhysics::FetchResults(const bool _bBlock)
{
	//if (false == m_bSimulate || nullptr == m_pScene)
	//	return;

	//while (false == (m_pScene->fetchResults(true)));

	////Callback 호출
	//for (auto& rPair : m_listCallback)
	//{
	//	if (false == rPair.bTrigger)
	//	{
	//		switch (rPair.ePairFlag)
	//		{
	//		case PxPairFlag::Enum::eNOTIFY_TOUCH_FOUND:
	//			if (rPair.pFirst.lock() && rPair.pSecond.lock())
	//			{
	//				rPair.pFirst.lock()->OnCollisionEnter(rPair.pSecond);
	//				rPair.pSecond.lock()->OnCollisionEnter(rPair.pFirst);
	//			}
	//			break;
	//		case PxPairFlag::Enum::eNOTIFY_TOUCH_PERSISTS:
	//			if (rPair.pFirst.lock() && rPair.pSecond.lock())
	//			{
	//				rPair.pFirst.lock()->OnCollisionStay(rPair.pSecond);
	//				rPair.pSecond.lock()->OnCollisionStay(rPair.pFirst);
	//			}
	//			break;
	//		case PxPairFlag::Enum::eNOTIFY_TOUCH_LOST:
	//			if (rPair.pFirst.lock() && rPair.pSecond.lock())
	//			{
	//				rPair.pFirst.lock()->OnCollisionExit(rPair.pSecond);
	//				rPair.pSecond.lock()->OnCollisionExit(rPair.pFirst);
	//			}
	//			break;
	//		default:
	//			break;
	//		}
	//	}
	//	else
	//	{
	//		switch (rPair.ePairFlag)
	//		{
	//		case PxPairFlag::Enum::eNOTIFY_TOUCH_FOUND:
	//			if (rPair.pFirst.lock() && rPair.pSecond.lock())
	//			{
	//				rPair.pFirst.lock()->OnTriggerEnter(rPair.pSecond);
	//				rPair.pSecond.lock()->OnTriggerEnter(rPair.pFirst);
	//			}
	//			break;
	//		case PxPairFlag::Enum::eNOTIFY_TOUCH_LOST:
	//			if (rPair.pFirst.lock() && rPair.pSecond.lock())
	//			{
	//				rPair.pFirst.lock()->OnTriggerExit(rPair.pSecond);
	//				rPair.pSecond.lock()->OnTriggerExit(rPair.pFirst);
	//			}
	//			break;
	//		default:
	//			break;
	//		}
	//	}
	//}

	//m_listCallback.clear();
	////Simulation 결과로 Actor에 설정된 globalPose를 Transform에 적용.
	//PxU32		nNumActors = 0;
	//PxActor** ppActors = nullptr;

	//nNumActors = m_pScene->getNbActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC);

	//ppActors = new PxActor * [nNumActors];

	//m_pScene->getActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC, ppActors, nNumActors);

	//std::weak_ptr<Collider> pCollider;
	//for (UINT i = 0; i < nNumActors; ++i)
	//{
	//	pCollider = ((LPPXUSERDATA)(ppActors[i]->userData))->pCollider;

	//	if (pCollider.expired())
	//		continue;

	//	physx::PxTransform globalPose = ppActors[i]->is<physx::PxRigidActor>()->getGlobalPose();
	//	pCollider.lock()->GetGameObject().lock()->GetComponent<Transform>().lock()->SetSimulationResult(D3DXQUATERNION(globalPose.q.x, globalPose.q.y, globalPose.q.z, globalPose.q.w), D3DXVECTOR3(globalPose.p.x, globalPose.p.y, globalPose.p.z));
	//}

	//delete[] ppActors;

	//m_bSimulate = false;
}

HRESULT GameEnginePhysics::CreateScene(const UINT _nSceneID)
{
	physx::PxScene* pScene = nullptr;

	physx::PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());

	sceneDesc.gravity = physx::PxVec3(0.f, -4.45f, 0.f);
	sceneDesc.cpuDispatcher = mDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;

	pScene = mPhysics->createScene(sceneDesc);

	if (nullptr == pScene)
	{
		MsgAssert("createScene 실패");
		return E_FAIL;
	}

	//Simulation Event Callback 등록.
	//pScene->setSimulationEventCallback(new CollisionCallback);

	//map container에 저장
	// mapScene.emplace(_nSceneID, pScene);

	return S_OK;

	//

	////Scene Description
	//physx::PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
	//sceneDesc.gravity = physx::PxVec3(0.f, -4.45f, 0.f);
	//sceneDesc.cpuDispatcher = mDispatcher;
	//sceneDesc.filterShader = contactReportFilterShader;

	////Create Scene
	//pScene = mPhysics->createScene(sceneDesc);

	//if (nullptr == pScene)
	//{
	//	MsgAssert("createScene 실패");
	//	return E_FAIL;
	//}

	////Simulation Event Callback 등록.
	//pScene->setSimulationEventCallback(new CollisionCallback);

	////map container에 저장
	//mapScene.emplace(_nSceneID, pScene);

	//return S_OK;
}

HRESULT GameEnginePhysics::ChangeScene(const UINT _nSceneID)
{
	//auto iterFind = mapScene.find(_nSceneID);

	////씬 아이디에 해당하는 피직스 씬이 없는 경우 피직스 씬 전환 실패.
	//if (mapScene.end() == iterFind)
	//{
	//	return E_FAIL;
	//}

	////이전 피직스 씬이 없는경우 
	////맵 컨테이너에서 찾은 씬을 현재 씬으로 설정.
	//if (nullptr == mScene)
	//{
	//	mScene = iterFind->second;
	//	return S_OK;
	//}

	////이전 피직스 씬이 있는경우 

	//// 1. 이전 피직스 씬에 남아있는 Actor를 현재 씬으로 옮김
	//PxU32		nNumActors = 0;
	//PxActor** ppActors = nullptr;

	////Scene에 있는 RigidDynamicActor의 개수
	//nNumActors = mScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC);
	////Scene에 있는 RigidDynamicActor 포인터를 받아오기 위한 동적 배열 생성
	//ppActors = new PxActor * [nNumActors];
	////Scene에서 RigidDynamicActor들을 가져옴.
	//m_pScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC, ppActors, nNumActors);
	////변경할 씬에 남아있는 Actor를 넘겨줌.
	//iterFind->second->addActors(ppActors, nNumActors);
	////생성한 동적 배열 삭제
	//delete[] ppActors;

	////Scene에 있는 RigidStaticActor의 숫자
	//nNumActors = m_pScene->getNbActors(PxActorTypeFlag::eRIGID_STATIC);
	////Scene에 있는 RigidStaticActor 포인터를 받아오기 위한 동적 배열 생성
	//ppActors = new PxActor * [nNumActors];
	////Scene에서 RigidStaticActor들을 가져옴.
	//m_pScene->getActors(PxActorTypeFlag::eRIGID_STATIC, ppActors, nNumActors);
	////변경할 씬에 남아있는 Actor를 넘겨줌.
	//iterFind->second->addActors(ppActors, nNumActors);
	////생성한 동적 배열 삭제
	//delete[] ppActors;

	//// 2. 이전 피직스 씬을 맵 컨테이너에서 제거
	//for (auto iterCurr = mapScene.begin(); iterCurr != m_mapScene.end(); ++iterCurr)
	//{
	//	if (iterCurr->second == m_pScene)
	//	{
	//		m_mapScene.erase(iterCurr);
	//		break;
	//	}
	//}

	//delete m_pScene->getSimulationEventCallback();

	//// 3. 이전 피직스 씬을 해제 한 후 맵 컨테이너에서 찾은 씬을 현재 씬으로 설정.
	//m_pScene->release();

	//m_pScene = iterFind->second;

	return S_OK;
}
