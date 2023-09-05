#include "PreCompileHeader.h"
#include "PhysXBoxComponent.h"

#include <GameEngineCore/GameEngineActor.h>
#include "GameEngineLevel.h"

PhysXBoxComponent::PhysXBoxComponent()
{
}

PhysXBoxComponent::~PhysXBoxComponent()
{
}

void PhysXBoxComponent::CreatePhysXActors(physx::PxVec3 _GeoMetryScale, float4 _GeoMetryRot)
{
	m_pPhysics = GetLevel()->GetLevelPhysics();
    m_pScene = GetLevel()->GetLevelScene();

	float4 tmpQuat = _GeoMetryRot.EulerDegToQuaternion();

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

	physx::PxVec3 tmpGeoMetryScale
	(
		_GeoMetryScale.x * 0.5f,
		_GeoMetryScale.y * 0.5f,
		_GeoMetryScale.z * 0.5f
	);

	// 충돌체의 종류
	//m_pDynamic = _physics->createRigidDynamic(localTm);
	m_pDynamic = m_pPhysics->createRigidDynamic(localTm);

	// 중력이 적용되지 않도록
	// TODO::RigidStatic으로 변경해야
	m_pDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
	m_pDynamic->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);

	// 충돌체의 형태
	m_pShape = physx::PxRigidActorExt::createExclusiveShape(*m_pDynamic, physx::PxBoxGeometry(tmpGeoMetryScale), *m_pMaterial);

	// RigidDynamic의 밀도를 설정
	physx::PxRigidBodyExt::updateMassAndInertia(*m_pDynamic, 0.1f);

	//피벗 설정
 	physx::PxVec3 Pivot(DynamicPivot.x, tmpGeoMetryScale.y, DynamicPivot.z);
	m_pShape->setLocalPose(physx::PxTransform(Pivot));

	//충돌할때 필요한 필터 데이터
	if (true == IsObstacle)
	{
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
	}
	else if (true == IsGround)
	{
		m_pShape->setSimulationFilterData
		(
			physx::PxFilterData
			(
				static_cast<physx::PxU32>(PhysXFilterGroup::Ground),
				static_cast<physx::PxU32>(PhysXFilterGroup::Player),
				0,
				0
			)
		);
	}
	else if (true == IsWall)
	{
		m_pShape->setSimulationFilterData
		(
			physx::PxFilterData
			(
				static_cast<physx::PxU32>(PhysXFilterGroup::Wall),
				static_cast<physx::PxU32>(PhysXFilterGroup::Player),
				0,
				0
			)
		);
	}
	else if (true == IsSlope)
	{
		m_pShape->setSimulationFilterData
		(
			physx::PxFilterData
			(
				static_cast<physx::PxU32>(PhysXFilterGroup::Slope),
				static_cast<physx::PxU32>(PhysXFilterGroup::Player),
				0,
				0
			)
		);
	}
	else
	{
		std::string ParentName = GetActor()->GetName().data();
		MsgTextBox("충돌 플래그가 설정되지 않았습니다. " + ParentName);
	}

	if (m_pScene == nullptr)
	{
		std::string LevelName = GetLevel()->GetName().data();
		MsgAssert("1. Start에서 피직스액터 생성하지 마세요\n2. 레벨에 CreateScene 하세요\n  오류가 뜬 레벨 이름 : " + LevelName);
	}

	// Scene에 액터 추가
	if (true == IsAggregateObject)
	{
		AddActorAggregate(m_pDynamic);
	}
	else
	{
		m_pScene->addActor(*m_pDynamic);
	}
}

void PhysXBoxComponent::Start()
{
	// 부모의 정보의 저장
	ParentActor = GetActor()->DynamicThis<GameEngineActor>();
}

void PhysXBoxComponent::Update(float _DeltaTime)
{
	if (false == PositionSetFromParentFlag)
	{
		// PhysX Actor의 상태에 맞춰서 부모의 Transform정보를 갱신
		float4 tmpWorldPos = 
		{ 
			m_pDynamic->getGlobalPose().p.x
			, m_pDynamic->getGlobalPose().p.y
			, m_pDynamic->getGlobalPose().p.z 
		};

		float4 EulerRot = PhysXDefault::GetQuaternionEulerAngles(m_pDynamic->getGlobalPose().q) * GameEngineMath::RadToDeg;

		ParentActor.lock()->GetTransform()->SetWorldRotation(float4{ EulerRot.x, EulerRot.y, EulerRot.z });
		ParentActor.lock()->GetTransform()->SetWorldPosition(tmpWorldPos);
	}
	else
	{
		float4 tmpQuat = ParentActor.lock()->GetTransform()->GetWorldRotation().EulerDegToQuaternion();

		physx::PxTransform tmpPxTransform
		(
			ParentActor.lock()->GetTransform()->GetWorldPosition().x,
			ParentActor.lock()->GetTransform()->GetWorldPosition().y,
			ParentActor.lock()->GetTransform()->GetWorldPosition().z,
			physx::PxQuat(tmpQuat.x, tmpQuat.y, tmpQuat.z, tmpQuat.w)
		);

		// 부모의 Transform정보를 바탕으로 PhysX Actor의 트랜스폼을 갱신
		m_pDynamic->setKinematicTarget(tmpPxTransform);
		//m_pDynamic->setGlobalPose(tmpPxTransform);
		// TODO::회전도 처리해야함. DegreeToQuat
	}
}
