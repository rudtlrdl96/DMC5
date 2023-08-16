#include "PreCompileHeader.h"
#include "PhysXBoxGeometryComponent.h"

#include <GameEngineCore/GameEngineActor.h>

PhysXBoxGeometryComponent::PhysXBoxGeometryComponent() 
{
}

PhysXBoxGeometryComponent::~PhysXBoxGeometryComponent() 
{
}

void PhysXBoxGeometryComponent::CreatePhysXActors(physx::PxScene* _Scene, physx::PxPhysics* _physics, physx::PxVec3 _GeoMetryScale, float4 _GeoMetryRot)
{
	float4 tmpQuat = _GeoMetryRot.EulerDegToQuaternion();

	// 부모 액터로부터 위치 생성
	physx::PxTransform localTm
	(
		physx::PxVec3
		(
			m_pParentActor.lock()->GetTransform()->GetWorldPosition().x
		, m_pParentActor.lock()->GetTransform()->GetWorldPosition().y
		, m_pParentActor.lock()->GetTransform()->GetWorldPosition().z
		),
		physx::PxQuat(tmpQuat.x, tmpQuat.y, tmpQuat.z, tmpQuat.w)
	);

	// 마찰, 탄성계수
	m_pMaterial = _physics->createMaterial(m_fStaticFriction, m_fDynamicFriction, m_fResitution);

	// TODO::배율을 적용할 경우 이쪽 코드를 사용
	//float4 tmpMagnification = { SIZE_MAGNIFICATION_RATIO };
	//physx::PxVec3 tmpGeoMetryScale(_GeoMetryScale.x * tmpMagnification.x * 0.5f, 
	//							   _GeoMetryScale.y * tmpMagnification.y * 0.5f, 
	//							   _GeoMetryScale.z * tmpMagnification.z * 0.5f);

	physx::PxVec3 tmpGeoMetryScale
	(
		_GeoMetryScale.x * 0.5f,
		_GeoMetryScale.y * 0.5f,
		_GeoMetryScale.z * 0.5f
	);

	// 충돌체의 종류
	m_pRigidDynamic = _physics->createRigidDynamic(localTm);

	// 중력이 적용되지 않도록
	// TODO::RigidStatic으로 변경해야
	m_pRigidDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);

	m_pRigidDynamic->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);

	// 충돌체의 형태
	// 충돌체의 크기는 절반의 크기를 설정하므로 실제 Renderer의 스케일은 충돌체의 2배로 설정되어야 함
	// TODO::부모 액터의 RenderUnit으로부터 Mesh의 Scale 과 WorldScale의 연산의 결과를 지오메트리의 Scale로 세팅해야함.
	m_pShape = physx::PxRigidActorExt::createExclusiveShape(*m_pRigidDynamic, physx::PxBoxGeometry(tmpGeoMetryScale), *m_pMaterial);

	// RigidDynamic의 밀도를 설정
	physx::PxRigidBodyExt::updateMassAndInertia(*m_pRigidDynamic, 0.1f);

	//피벗 설정
	physx::PxVec3 Pivot(m_f4DynamicPivot.x, m_f4DynamicPivot.y, m_f4DynamicPivot.z);
	m_pShape->setLocalPose(physx::PxTransform(Pivot));

	//충돌할때 필요한 필터 데이터
	if (m_bObstacle == true)
	{
		m_pShape->setSimulationFilterData
		(
			physx::PxFilterData(static_cast<physx::PxU32>(PhysXFilterGroup::Obstacle)
			, static_cast<physx::PxU32>(PhysXFilterGroup::PlayerDynamic), 0, 0)
		);
	}
	else if (m_bGround == true)
	{
		m_pShape->setSimulationFilterData
		(
			physx::PxFilterData(static_cast<physx::PxU32>(PhysXFilterGroup::Ground)
			, static_cast<physx::PxU32>(PhysXFilterGroup::PlayerDynamic), 0, 0)
		);
	}

	//콜백피벗 설정
	m_pShape->setLocalPose(physx::PxTransform(Pivot));

	// Scene에 액터 추가
	if (true == m_bAggregateObj)
	{
		AddActorAggregate(m_pRigidDynamic);
	}
	else
	{
		_Scene->addActor(*m_pRigidDynamic);
	}

	//// 충돌체의 종류
	//rigidStatic_ = _physics->createRigidStatic(localTm);
	//rigidStatic_->attachShape(*shape_);

	//// Scene에 액터 추가
	//_Scene->addActor(*rigidStatic_);
}

void PhysXBoxGeometryComponent::Start()
{
	// 부모의 정보의 저장
	m_pParentActor = GetActor()->DynamicThis<GameEngineActor>();
}

void PhysXBoxGeometryComponent::Update(float _DeltaTime)
{
	if (false == m_bPositionSetFromParentFlag)
	{
		// PhysX Actor의 상태에 맞춰서 부모의 Transform정보를 갱신
		float4 tmpWorldPos = 
		{ 
			m_pRigidDynamic->getGlobalPose().p.x
			, m_pRigidDynamic->getGlobalPose().p.y
			, m_pRigidDynamic->getGlobalPose().p.z 
		};

		float4 EulerRot = PhysXDefault::GetQuaternionEulerAngles(m_pRigidDynamic->getGlobalPose().q) * GameEngineMath::RadToDeg;

		m_pParentActor.lock()->GetTransform()->SetWorldRotation(float4{ EulerRot.x, EulerRot.y, EulerRot.z });
		m_pParentActor.lock()->GetTransform()->SetWorldPosition(tmpWorldPos);
	}
	else
	{
		float4 tmpQuat = m_pParentActor.lock()->GetTransform()->GetWorldRotation().EulerDegToQuaternion();

		physx::PxTransform tmpPxTransform
		(
			m_pParentActor.lock()->GetTransform()->GetWorldPosition().x,
			m_pParentActor.lock()->GetTransform()->GetWorldPosition().y,
			m_pParentActor.lock()->GetTransform()->GetWorldPosition().z, 
			physx::PxQuat(tmpQuat.x, tmpQuat.y, tmpQuat.z, tmpQuat.w)
		);

		// 부모의 Transform정보를 바탕으로 PhysX Actor의 트랜스폼을 갱신
		m_pRigidDynamic->setKinematicTarget(tmpPxTransform);
		// TODO::회전도 처리해야함. DegreeToQuat
	}
}
