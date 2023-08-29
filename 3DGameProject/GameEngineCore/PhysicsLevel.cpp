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


// �޸�����
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

physx::PxFilterFlags CustomFilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0, physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1, physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
{
	// SampleSubmarineFilterShader�κ��� ������
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
		MsgAssert("PxFoundation ���� ����");
	}

#ifdef _DEBUG
	m_pPvd = PxCreatePvd(*m_pFoundation);
	if (!m_pPvd)
	{
		MsgAssert("PxPvd ���� ����");
	}

	m_pTransport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	if (!m_pTransport)
	{
		MsgAssert("Transport ���� ����");
	}

	m_pPvd->connect(*m_pTransport, physx::PxPvdInstrumentationFlag::eALL);

#endif
	physx::PxTolerancesScale TolerancesScale;

	//TolerancesScale.length = 1.f;
	//TolerancesScale.speed = 10.f;

	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, TolerancesScale, true, m_pPvd);
	if (!m_pPhysics)
	{
		MsgAssert("PxPhysics ���� ����");
	}

	physx::PxSceneDesc SceneDesc(m_pPhysics->getTolerancesScale());

	SceneDesc.gravity = physx::PxVec3(0.0f, -1000.0f, 0.0f);
	m_pDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	SceneDesc.cpuDispatcher = m_pDispatcher;
	// EventCallback ����
	CustomCallbackEvent = new CustomCallback();
	SceneDesc.simulationEventCallback = CustomCallbackEvent;
	// callback�� ȣ�� ó���� filtershader ����
	SceneDesc.filterShader = CustomFilterShader;

	m_pScene = m_pPhysics->createScene(SceneDesc);
	if (!m_pScene)
	{
		MsgAssert("PxScene ���� ����");
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
		MsgAssert("PxCooking ���� ����");
	}

	StepSize = 1.0f / 60.0f;

	// Aggregate����
	//MapAggregate_ = Physics_->createAggregate(2000, false);
	//Scene_->addAggregate(*MapAggregate_);
}

void PhysicsLevel::advance(physx::PxReal _DeltaTime)
{
	//WaitTime += _DeltaTime;
	//StepSize = 1.0f / 60.0f;

	//if (WaitTime < StepSize)
	//{
	//	IsSimulation = false;
	//}
	//else
	//{
	//	WaitTime -= StepSize;

	//	m_pScene->simulate(StepSize);

	//	IsSimulation = true;
	//}
}

// ������ ���������� ����
void PhysicsLevel::Simulate(float _DeltaTime)
{
	if (nullptr == m_pPhysics || 
		true == IsPhysics ||
		0.0f >= _DeltaTime)
	{
		return;
	}

	m_pScene->simulate(_DeltaTime);
	m_pScene->fetchResults(true);
}

bool PhysicsLevel::RayCast(const float4& _vOrigin, const float4& _vDir, OUT float4& _vPoint, float _fDistance)
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

//void Collider::ReadySimulate()
//{
//	std::weak_ptr<Transform> pTransform = m_pGameObject.lock()->GetComponent<Transform>();

//	if (true == pTransform.expired())
//		return;
//	if (false == pTransform.lock()->IsUpdated())
//		return;
//	if (nullptr == m_pRigidActor)
//		return;
//
//
//	D3DXVECTOR3		vPosition = pTransform.lock()->GetPosition();
//	D3DXQUATERNION	tQuaternion = pTransform.lock()->GetQuaternion();
//
//	PxVec3 pxPos;
//	PxQuat pxQuat;
//
//	memcpy_s(&pxPos, sizeof(D3DXVECTOR3), &vPosition, sizeof(D3DXVECTOR3));
//	memcpy_s(&pxQuat, sizeof(D3DXQUATERNION), &tQuaternion, sizeof(D3DXQUATERNION));
//
//	if (pTransform.lock()->IsSetPosition() || !m_bRigid || m_bTrigger)
//	{
//		m_pRigidActor->setGlobalPose(physx::PxTransform(pxPos, pxQuat));
//		return;
//	}
//
//	PxVec3 vCurrPos = m_pRigidActor->getGlobalPose().p;
//
//	PxVec3 vRayDir = pxPos - vCurrPos;
//
//	float fDistance = vRayDir.magnitude();
//
//	vRayDir = vRayDir.getNormalized();
//
//	PxVec3 vOrigin = vCurrPos + m_pShape->getLocalPose().p;
//
//	PxRaycastBuffer rayCastBuffer;
//
//	PxScene* pScene = m_pRigidActor->getScene();
//
//	if (PxVec3(0) != vRayDir && pScene->raycast(vOrigin, vRayDir, 100.f, rayCastBuffer))
//	{
//		if (rayCastBuffer.block.distance < fDistance)
//		{
//			pxPos = m_pRigidActor->getGlobalPose().p + vRayDir * (rayCastBuffer.block.distance - m_fOffsetRadius);
//		}
//	}
//
//	m_pRigidActor->setGlobalPose(physx::PxTransform(pxPos, pxQuat));
//}