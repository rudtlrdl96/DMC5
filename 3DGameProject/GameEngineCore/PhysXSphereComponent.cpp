#include "PrecompileHeader.h"
#include "PhysXSphereComponent.h"

#include "GameEngineLevel.h"

PhysXSphereComponent::PhysXSphereComponent()
{
}

PhysXSphereComponent::~PhysXSphereComponent()
{
}

void PhysXSphereComponent::CreatePhysXActors(physx::PxScene* _Scene, physx::PxPhysics* _physics, physx::PxVec3 _GeoMetryScale, float4 _GeoMetryRotation)
{
	m_pPhysics = GetLevel()->GetLevelPhysics();
	m_pScene = GetLevel()->GetLevelScene();

	float4 tmpQuat = _GeoMetryRotation.EulerDegToQuaternion();

	// 부모 액터로부터 위치 생성
	physx::PxTransform localTm
	(
		physx::PxVec3
		(
			ParentActor.lock()->GetTransform()->GetWorldPosition().x
			, ParentActor.lock()->GetTransform()->GetWorldPosition().y
			, ParentActor.lock()->GetTransform()->GetWorldPosition().z
		),
		physx::PxQuat(tmpQuat.x, tmpQuat.y, tmpQuat.z, tmpQuat.w)
	);

	// Staticfriction : 정적마찰 // Dynamicfriction : 동적마찰 // Resitution : 탄성계수
	m_pMaterial = m_pPhysics->createMaterial(Staticfriction, Dynamicfriction, Resitution);

	// TODO::배율을 적용할 경우 이쪽 코드를 사용
	//float4 tmpMagnification = { SIZE_MAGNIFICATION_RATIO };
	//physx::PxVec3 tmpGeoMetryScale(_GeoMetryScale.x * tmpMagnification.x * 0.5f, 
	//							   _GeoMetryScale.y * tmpMagnification.y * 0.5f, 
	//							   _GeoMetryScale.z * tmpMagnification.z * 0.5f);

	//GeoMetryScale = _GeoMetryScale;

	physx::PxVec3 tmpGeoMetryScale
	(
		_GeoMetryScale.x * 0.5f,
		_GeoMetryScale.y * 0.5f,
		_GeoMetryScale.z * 0.5f
	);

	// 충돌체의 종류
	m_pDynamic = m_pPhysics->createRigidDynamic(localTm);

	// 특정 축을 따라/주위로 동작을 잠그는 메커니즘을 제공하는 플래그 모음
	m_pDynamic->setRigidDynamicLockFlags
	(
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X |
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y |
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z
	);

	// TODO::점프속력에 영향을 미침. 스테이지 작성후 자세한 수치는 나중에 조절
	// 플레이어 최대 속력
	// dynamic_->setMaxLinearVelocity(PLAYER_MAX_SPEED);

	float ScaledRadius = tmpGeoMetryScale.z;
	float ScaledHeight = tmpGeoMetryScale.y;

	// 충돌체의 형태
	// 충돌체의 크기는 절반의 크기를 설정하므로 실제 Renderer의 스케일은 충돌체의 2배로 설정되어야 함
	// TODO::부모 액터의 RenderUnit으로부터 Mesh의 Scale 과 WorldScale의 연산의 결과를 지오메트리의 Scale로 세팅해야함.
	m_pShape = physx::PxRigidActorExt::createExclusiveShape(*m_pDynamic, physx::PxSphereGeometry(physx::PxReal(_GeoMetryScale.y * 0.5f)), *m_pMaterial);

	// RigidDynamic의 밀도를 설정
	//physx::PxRigidBodyExt::updateMassAndInertia(*m_pDynamic, 0.01f);

	//피벗 설정
	float CapsuleHeight = ScaledHeight * 1.f;
	physx::PxVec3 DynamicCenter = physx::PxVec3{ 0.0f, CapsuleHeight, 0.0f };
	physx::PxTransform relativePose(physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0, 0, 1)));
	relativePose.p = DynamicCenter;
	m_pShape->setLocalPose(relativePose);

	//충돌할때 필요한 필터 데이터
	m_pShape->setSimulationFilterData
	(
		physx::PxFilterData
		(
			static_cast<physx::PxU32>(PhysXFilterGroup::Obstacle),
			static_cast<physx::PxU32>(PhysXFilterGroup::Player), 
			0,
			0
		)
	);
	//m_pShape->setSimulationFilterData(physx::PxFilterData(static_cast<physx::PxU32>(PhysXFilterGroup::PlayerDynamic), static_cast<physx::PxU32>(PhysXFilterGroup::Ground), static_cast<physx::PxU32>(PhysXFilterGroup::Obstacle), 0));

	m_pShape->setContactOffset(0.2f);
	
	// 제동?
	//m_pDynamic->setLinearDamping(physx::PxReal(0.01f));
	//m_pDynamic->setMaxAngularVelocity(physx::PxReal(20.0f));
	//m_pDynamic->setAngularDamping(physx::PxReal(2.0f));

	if (m_pScene == nullptr)
	{
		std::string LevelName = GetLevel()->GetName().data();
		MsgAssert("1. Start에서 피직스액터 생성하지 마세요\n2. 레벨에 CreateScene 하세요\n  오류가 뜬 레벨 이름 : " + LevelName);
	}

	PhysicsComponent = DynamicThis<PhysXSphereComponent>();
	m_pShape->userData = GetActor();

	GetActor()->SetPhysicsActor();

	// Scene에 액터 추가
	m_pScene->addActor(*m_pDynamic);
}

void PhysXSphereComponent::Start()
{
	// 부모의 정보의 저장
	ParentActor = GetActor()->DynamicThis<GameEngineActor>();
}

void PhysXSphereComponent::Update(float _DeltaTime)
{
	if (!(physx::PxIsFinite(m_pDynamic->getGlobalPose().p.x) || physx::PxIsFinite(m_pDynamic->getGlobalPose().p.y) || physx::PxIsFinite(m_pDynamic->getGlobalPose().p.z))
		&& true == IsMain)
	{
		m_pDynamic->setGlobalPose(RecentTransform);
	}

	// PhysX Actor의 상태에 맞춰서 부모의 Transform정보를 갱신
	float4 tmpWorldPos = { m_pDynamic->getGlobalPose().p.x, m_pDynamic->getGlobalPose().p.y, m_pDynamic->getGlobalPose().p.z };
	float4 EulerRot = PhysXDefault::GetQuaternionEulerAngles(m_pDynamic->getGlobalPose().q) * GameEngineMath::RadToDeg;

	ParentActor.lock()->GetTransform()->SetWorldRotation(float4{ EulerRot.x, EulerRot.y, EulerRot.z });
	ParentActor.lock()->GetTransform()->SetWorldPosition(tmpWorldPos);
}

void PhysXSphereComponent::ResetDynamic()
{
	float4 tmpQuat = float4{ 0.0f,0.0f,0.0f }.EulerDegToQuaternion();
	const physx::PxQuat tmpPxQuat(tmpQuat.x, tmpQuat.y, tmpQuat.z, tmpQuat.w);
	const physx::PxTransform tmpTansform(m_pDynamic->getGlobalPose().p, tmpPxQuat);

	m_pDynamic->setGlobalPose(tmpTansform);
	m_pDynamic->setRigidDynamicLockFlags(
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X |
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y |
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
}


