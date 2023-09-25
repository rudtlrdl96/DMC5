#include "PreCompileHeader.h"
#include "PhysXBoxComponent.h"

#include <GameEngineCore/GameEngineActor.h>
#include "GameEngineActor.h"
#include "GameEngineLevel.h"

PhysXBoxComponent::PhysXBoxComponent()
{
}

PhysXBoxComponent::~PhysXBoxComponent()
{
}

void PhysXBoxComponent::Start()
{
	// 부모의 정보의 저장
	ParentActor = GetActor()->DynamicThis<GameEngineActor>();
}

void PhysXBoxComponent::Update(float _DeltaTime)
{
	if (true == PositionSetFromParentFlag)
	{
		// 부모의 Transform정보를 바탕으로 PhysX Actor의 트랜스폼을 갱신
		float4 tmpQuat = ParentActor.lock()->GetTransform()->GetWorldRotation().DegreeRotationToQuaternionReturn();

		physx::PxTransform tmpPxTransform
		(
			ParentActor.lock()->GetTransform()->GetWorldPosition().x,
			ParentActor.lock()->GetTransform()->GetWorldPosition().y,
			ParentActor.lock()->GetTransform()->GetWorldPosition().z,
			physx::PxQuat(tmpQuat.x, tmpQuat.y, tmpQuat.z, tmpQuat.w)
		);

		//m_pDynamic->setKinematicTarget(tmpPxTransform);
		m_pStatic->setGlobalPose(tmpPxTransform);
	}
	else
	{
		// PhysX Actor의 상태에 맞춰서 부모의 Transform정보를 갱신
		float4 tmpWorldPos =
		{
			m_pStatic->getGlobalPose().p.x
			, m_pStatic->getGlobalPose().p.y
			, m_pStatic->getGlobalPose().p.z
		};

		float4 EulerRot = PhysXDefault::GetQuaternionEulerAngles(m_pStatic->getGlobalPose().q) * GameEngineMath::RadToDeg;

		ParentActor.lock()->GetTransform()->SetWorldRotation(float4{ EulerRot.x, EulerRot.y, EulerRot.z });
		ParentActor.lock()->GetTransform()->SetWorldPosition(tmpWorldPos);
	}
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
	m_pStatic = m_pPhysics->createRigidStatic(localTm);

	// 중력이 적용되지 않도록
	// TODO::RigidStatic으로 변경해야
	//m_pDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
	//m_pDynamic->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);

	// 충돌체의 형태
	m_pShape = physx::PxRigidActorExt::createExclusiveShape(*m_pStatic, physx::PxBoxGeometry(tmpGeoMetryScale), *m_pMaterial);

	// RigidDynamic의 밀도를 설정
	//physx::PxRigidBodyExt::updateMassAndInertia(*m_pDynamic, 0.1f);

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

	PhysicsComponent = DynamicThis<PhysXBoxComponent>();
	m_pShape->userData = GetActor();

	GetActor()->SetPhysicsActor();

	// Scene에 액터 추가
	if (true == IsAggregateObject)
	{
		AddActorAggregate(m_pStatic);
	}
	else
	{
		m_pScene->addActor(*m_pStatic);
	}
}

void PhysXBoxComponent::SetWorldPosition(float4 _Value)
{
	physx::PxTransform CurTansform = m_pStatic->getGlobalPose();

	float ValueX = _Value.x;
	float ValueY = _Value.y;
	float ValueZ = _Value.z;

	CurTansform.p = { _Value.x, _Value.y, _Value.z };

	m_pStatic->setGlobalPose(CurTansform);
}

void PhysXBoxComponent::AddWorldPosition(float4 _Value)
{
	physx::PxTransform CurTansform = m_pStatic->getGlobalPose();

	float ValueX = _Value.x;
	float ValueY = _Value.y;
	float ValueZ = _Value.z;

	CurTansform.p += { _Value.x, _Value.y, _Value.z };

	m_pStatic->setGlobalPose(CurTansform);
}

void PhysXBoxComponent::SetWorldRotation(float4 _Value)
{
	float4 tmpQuat = _Value.DegreeRotationToQuaternionReturn();

	const physx::PxQuat tmpPxQuat(tmpQuat.x, tmpQuat.y, tmpQuat.z, tmpQuat.w);
	const physx::PxTransform tmpTansform(m_pStatic->getGlobalPose().p, tmpPxQuat);

	m_pStatic->setGlobalPose(tmpTansform);
}

void PhysXBoxComponent::AddWorldRotation(float4 _Value)
{
	float4 AddRptation = _Value;
	physx::PxQuat CurQuat = m_pStatic->getGlobalPose().q;
	float4 CurQuatToFloat4 = { CurQuat.x , CurQuat.y, CurQuat.z, CurQuat.w };
	float4 CurRotation = CurQuatToFloat4.QuaternionToEulerDeg();
	float4 ResultRotation = { CurRotation.x + AddRptation.x, CurRotation.y + AddRptation.y, CurRotation.z + AddRptation.z, CurRotation.w + AddRptation.w };

	float4 ResultsQuat = ResultRotation.DegreeRotationToQuaternionReturn();
	const physx::PxQuat tmpPxQuat(ResultsQuat.x, ResultsQuat.y, ResultsQuat.z, ResultsQuat.w);
	const physx::PxTransform tmpTansform(m_pStatic->getGlobalPose().p, tmpPxQuat);

	m_pStatic->setGlobalPose(tmpTansform);
}