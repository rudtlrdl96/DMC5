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

void PhysXBoxComponent::CreatePhysXActors(physx::PxVec3 _GeoMetryScale, float4 _GeoMetryRot)
{
	m_pPhysics = GetLevel()->GetLevelPhysics();
    m_pScene = GetLevel()->GetLevelScene();

	float4 tmpQuat = _GeoMetryRot.EulerDegToQuaternion();

	// �θ� ���ͷκ��� ��ġ ����
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

	// Staticfriction : �������� // Dynamicfriction : �������� // Resitution : ź�����
	m_pMaterial = m_pPhysics->createMaterial(Staticfriction, Dynamicfriction, Resitution);

	physx::PxVec3 tmpGeoMetryScale
	(
		_GeoMetryScale.x * 0.5f,
		_GeoMetryScale.y * 0.5f,
		_GeoMetryScale.z * 0.5f
	);

	// �浹ü�� ����
	//m_pDynamic = _physics->createRigidDynamic(localTm);
	m_pDynamic = m_pPhysics->createRigidDynamic(localTm);

	// �߷��� ������� �ʵ���
	// TODO::RigidStatic���� �����ؾ�
	m_pDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
	m_pDynamic->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);

	// �浹ü�� ����
	m_pShape = physx::PxRigidActorExt::createExclusiveShape(*m_pDynamic, physx::PxBoxGeometry(tmpGeoMetryScale), *m_pMaterial);

	// RigidDynamic�� �е��� ����
	physx::PxRigidBodyExt::updateMassAndInertia(*m_pDynamic, 0.1f);

	//�ǹ� ����
 	physx::PxVec3 Pivot(DynamicPivot.x, tmpGeoMetryScale.y, DynamicPivot.z);
	m_pShape->setLocalPose(physx::PxTransform(Pivot));

	//�浹�Ҷ� �ʿ��� ���� ������
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
		MsgTextBox("�浹 �÷��װ� �������� �ʾҽ��ϴ�. " + ParentName);
	}

	if (m_pScene == nullptr)
	{
		std::string LevelName = GetLevel()->GetName().data();
		MsgAssert("1. Start���� ���������� �������� ������\n2. ������ CreateScene �ϼ���\n  ������ �� ���� �̸� : " + LevelName);
	}

	PhysicsComponent = DynamicThis<PhysXBoxComponent>();
	m_pShape->userData = GetActor();

	GetActor()->SetPhysicsActor();

	// Scene�� ���� �߰�
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
	// �θ��� ������ ����
	ParentActor = GetActor()->DynamicThis<GameEngineActor>();
}

void PhysXBoxComponent::Update(float _DeltaTime)
{
	if (false == PositionSetFromParentFlag)
	{
		// PhysX Actor�� ���¿� ���缭 �θ��� Transform������ ����
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

		// �θ��� Transform������ �������� PhysX Actor�� Ʈ�������� ����
		m_pDynamic->setKinematicTarget(tmpPxTransform);
		//m_pDynamic->setGlobalPose(tmpPxTransform);
		// TODO::ȸ���� ó���ؾ���. DegreeToQuat
	}
}
