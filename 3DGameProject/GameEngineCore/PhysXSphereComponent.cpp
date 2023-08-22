#include "PreCompileHeader.h"
#include "PhysXSphereComponent.h"

#include <GameEngineCore/GameEngineActor.h>

PhysXSphereComponent::PhysXSphereComponent() 
{
}

PhysXSphereComponent::~PhysXSphereComponent() 
{
}

void PhysXSphereComponent::CreatePhysXActors(physx::PxScene* _Scene, physx::PxPhysics* _physics, physx::PxVec3 _GeoMetryScale, float4 _GeoMetryRot)
{
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
	m_pMaterial = _physics->createMaterial(Staticfriction, Dynamicfriction, Resitution);

	// TODO::������ ������ ��� ���� �ڵ带 ���
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

	// �浹ü�� ����
	m_pRigidDynamic = _physics->createRigidDynamic(localTm);

	// �߷��� ������� �ʵ���
	// TODO::RigidStatic���� �����ؾ�
	//m_pRigidDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
	//m_pRigidDynamic->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);

	// �浹ü�� ����
	// �浹ü�� ũ��� ������ ũ�⸦ �����ϹǷ� ���� Renderer�� �������� �浹ü�� 2��� �����Ǿ�� ��
	// TODO::�θ� ������ RenderUnit���κ��� Mesh�� Scale �� WorldScale�� ������ ����� ������Ʈ���� Scale�� �����ؾ���.
	m_pShape = physx::PxRigidActorExt::createExclusiveShape(*m_pRigidDynamic, physx::PxBoxGeometry(tmpGeoMetryScale), *m_pMaterial);

	// RigidDynamic�� �е��� ����
	physx::PxRigidBodyExt::updateMassAndInertia(*m_pRigidDynamic, 0.1f);

	//�ǹ� ����
	physx::PxVec3 Pivot(DynamicPivot.x, DynamicPivot.y, DynamicPivot.z);
	m_pShape->setLocalPose(physx::PxTransform(Pivot));

	//�浹�Ҷ� �ʿ��� ���� ������
	if (IsObstacle == true)
	{
		m_pShape->setSimulationFilterData
		(
			physx::PxFilterData(static_cast<physx::PxU32>(PhysXFilterGroup::Obstacle)
				, static_cast<physx::PxU32>(PhysXFilterGroup::PlayerDynamic), 0, 0)
		);
	}
	else if (IsGround == true)
	{
		m_pShape->setSimulationFilterData
		(
			physx::PxFilterData(static_cast<physx::PxU32>(PhysXFilterGroup::Ground)
				, static_cast<physx::PxU32>(PhysXFilterGroup::PlayerDynamic), 0, 0)
		);
	}

	//�ݹ��ǹ� ����
	m_pShape->setLocalPose(physx::PxTransform(Pivot));

	// Scene�� ���� �߰�
	if (true == IsAggregateObject)
	{
		AddActorAggregate(m_pRigidDynamic);
	}
	else
	{
		_Scene->addActor(*m_pRigidDynamic);
	}
}

void PhysXSphereComponent::Start()
{
	// �θ��� ������ ����
	ParentActor = GetActor()->DynamicThis<GameEngineActor>();
}

void PhysXSphereComponent::Update(float _DeltaTime)
{
	if (false == PositionSetFromParentFlag)
	{
		// PhysX Actor�� ���¿� ���缭 �θ��� Transform������ ����
		float4 tmpWorldPos =
		{
			m_pRigidDynamic->getGlobalPose().p.x,
			m_pRigidDynamic->getGlobalPose().p.y,
			m_pRigidDynamic->getGlobalPose().p.z
		};

		float4 EulerRot = PhysXDefault::GetQuaternionEulerAngles(m_pRigidDynamic->getGlobalPose().q) * GameEngineMath::RadToDeg;

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
		m_pRigidDynamic->setKinematicTarget(tmpPxTransform);
		//m_pRigidDynamic->setGlobalPose(tmpPxTransform);
		// TODO::ȸ���� ó���ؾ���. DegreeToQuat
	}
}
