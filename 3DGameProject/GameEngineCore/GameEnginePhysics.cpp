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
	const int numThreads = 4; // ���ϴ� ������ ����
	physx::PxDefaultCpuDispatcher* cpuDispatcher = nullptr;
	//physx::PxDefaultCpuDispatcher* cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(numThreads);
	return cpuDispatcher;
}

void GameEnginePhysics::Initialize()
{
	mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
	if (nullptr == mFoundation)
	{
		MsgAssert("PxCreateFoundation ����");
	}

#ifdef _DEBUG
	//mPVD = PxCreatePvd(*mFoundation);
	//if (nullptr == mPVD)
	//{
	//	MsgAssert("PxCreatePvd ����");
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
		MsgAssert("PxCreatePhysics ����");
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

	////Simulation ���� �� ����� Transform�� Actor�� ����
	//PxU32		nNumActors = 0;
	//PxActor** ppActors = nullptr;

	////Scene�� �ִ� RigidDynamicActor�� ����
	//nNumActors = m_pScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC);
	////Scene�� �ִ� RigidDynamicActor �����͸� �޾ƿ��� ���� ���� �迭 ����
	//ppActors = new PxActor * [nNumActors];
	////Scene���� RigidDynamicActor���� ������.
	//m_pScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC, ppActors, nNumActors);
	////
	//for (UINT i = 0; i < nNumActors; ++i)
	//{
	//	if (ppActors[i]->userData)
	//		((LPPXUSERDATA)(ppActors[i]->userData))->pCollider.lock()->ReadySimulate();
	//}
	////������ ���� �迭 ����
	//delete[] ppActors;

	////Scene�� �ִ� RigidStaticActor�� ����
	//nNumActors = m_pScene->getNbActors(PxActorTypeFlag::eRIGID_STATIC);
	////Scene�� �ִ� RigidStaticActor �����͸� �޾ƿ��� ���� ���� �迭 ����
	//ppActors = new PxActor * [nNumActors];
	////Scene���� RigidStaticActor���� ������.
	//m_pScene->getActors(PxActorTypeFlag::eRIGID_STATIC, ppActors, nNumActors);
	////
	//for (UINT i = 0; i < nNumActors; ++i)
	//{
	//	if (ppActors[i]->userData)
	//		((LPPXUSERDATA)(ppActors[i]->userData))->pCollider.lock()->ReadySimulate();
	//}
	////������ ���� �迭 ����
	//delete[] ppActors;

	////Simulation ����
	//m_pScene->simulate(1.f / 60.f);

	//m_bSimulate = true;
}

void GameEnginePhysics::FetchResults(const bool _bBlock)
{
	//if (false == m_bSimulate || nullptr == m_pScene)
	//	return;

	//while (false == (m_pScene->fetchResults(true)));

	////Callback ȣ��
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
	////Simulation ����� Actor�� ������ globalPose�� Transform�� ����.
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
		MsgAssert("createScene ����");
		return E_FAIL;
	}

	//Simulation Event Callback ���.
	//pScene->setSimulationEventCallback(new CollisionCallback);

	//map container�� ����
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
	//	MsgAssert("createScene ����");
	//	return E_FAIL;
	//}

	////Simulation Event Callback ���.
	//pScene->setSimulationEventCallback(new CollisionCallback);

	////map container�� ����
	//mapScene.emplace(_nSceneID, pScene);

	//return S_OK;
}

HRESULT GameEnginePhysics::ChangeScene(const UINT _nSceneID)
{
	//auto iterFind = mapScene.find(_nSceneID);

	////�� ���̵� �ش��ϴ� ������ ���� ���� ��� ������ �� ��ȯ ����.
	//if (mapScene.end() == iterFind)
	//{
	//	return E_FAIL;
	//}

	////���� ������ ���� ���°�� 
	////�� �����̳ʿ��� ã�� ���� ���� ������ ����.
	//if (nullptr == mScene)
	//{
	//	mScene = iterFind->second;
	//	return S_OK;
	//}

	////���� ������ ���� �ִ°�� 

	//// 1. ���� ������ ���� �����ִ� Actor�� ���� ������ �ű�
	//PxU32		nNumActors = 0;
	//PxActor** ppActors = nullptr;

	////Scene�� �ִ� RigidDynamicActor�� ����
	//nNumActors = mScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC);
	////Scene�� �ִ� RigidDynamicActor �����͸� �޾ƿ��� ���� ���� �迭 ����
	//ppActors = new PxActor * [nNumActors];
	////Scene���� RigidDynamicActor���� ������.
	//m_pScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC, ppActors, nNumActors);
	////������ ���� �����ִ� Actor�� �Ѱ���.
	//iterFind->second->addActors(ppActors, nNumActors);
	////������ ���� �迭 ����
	//delete[] ppActors;

	////Scene�� �ִ� RigidStaticActor�� ����
	//nNumActors = m_pScene->getNbActors(PxActorTypeFlag::eRIGID_STATIC);
	////Scene�� �ִ� RigidStaticActor �����͸� �޾ƿ��� ���� ���� �迭 ����
	//ppActors = new PxActor * [nNumActors];
	////Scene���� RigidStaticActor���� ������.
	//m_pScene->getActors(PxActorTypeFlag::eRIGID_STATIC, ppActors, nNumActors);
	////������ ���� �����ִ� Actor�� �Ѱ���.
	//iterFind->second->addActors(ppActors, nNumActors);
	////������ ���� �迭 ����
	//delete[] ppActors;

	//// 2. ���� ������ ���� �� �����̳ʿ��� ����
	//for (auto iterCurr = mapScene.begin(); iterCurr != m_mapScene.end(); ++iterCurr)
	//{
	//	if (iterCurr->second == m_pScene)
	//	{
	//		m_mapScene.erase(iterCurr);
	//		break;
	//	}
	//}

	//delete m_pScene->getSimulationEventCallback();

	//// 3. ���� ������ ���� ���� �� �� �� �����̳ʿ��� ã�� ���� ���� ������ ����.
	//m_pScene->release();

	//m_pScene = iterFind->second;

	return S_OK;
}
