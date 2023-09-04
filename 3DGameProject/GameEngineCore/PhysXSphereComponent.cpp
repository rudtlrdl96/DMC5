#include "PrecompileHeader.h"
#include "PhysXSphereComponent.h"

#include "GameEngineLevel.h"

PhysXSphereComponent::PhysXSphereComponent()
{
}

PhysXSphereComponent::~PhysXSphereComponent()
{
}

physx::PxRigidDynamic* PhysXSphereComponent::CreatePhysXActors(physx::PxScene* _Scene, physx::PxPhysics* _physics, physx::PxVec3 _GeoMetryScale, float4 _GeoMetryRotation)
{
	m_pPhysics = GetLevel()->GetLevelPhysics();
	m_pScene = GetLevel()->GetLevelScene();

	float4 tmpQuat = _GeoMetryRotation.EulerDegToQuaternion();

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

	// TODO::������ ������ ��� ���� �ڵ带 ���
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

	// �浹ü�� ����
	m_pRigidDynamic = m_pPhysics->createRigidDynamic(localTm);

	// Ư�� ���� ����/������ ������ ��״� ��Ŀ������ �����ϴ� �÷��� ����
	m_pRigidDynamic->setRigidDynamicLockFlags
	(
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X |
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y |
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z
	);

	// TODO::�����ӷ¿� ������ ��ħ. �������� �ۼ��� �ڼ��� ��ġ�� ���߿� ����
	// �÷��̾� �ִ� �ӷ�
	// dynamic_->setMaxLinearVelocity(PLAYER_MAX_SPEED);

	float ScaledRadius = tmpGeoMetryScale.z;
	float ScaledHeight = tmpGeoMetryScale.y;

	// �浹ü�� ����
	// �浹ü�� ũ��� ������ ũ�⸦ �����ϹǷ� ���� Renderer�� �������� �浹ü�� 2��� �����Ǿ�� ��
	// TODO::�θ� ������ RenderUnit���κ��� Mesh�� Scale �� WorldScale�� ������ ����� ������Ʈ���� Scale�� �����ؾ���.
	m_pShape = physx::PxRigidActorExt::createExclusiveShape(*m_pRigidDynamic, physx::PxSphereGeometry(physx::PxReal(_GeoMetryScale.y * 0.5f)), *m_pMaterial);

	// RigidDynamic�� �е��� ����
	//physx::PxRigidBodyExt::updateMassAndInertia(*m_pRigidDynamic, 0.01f);

	//�ǹ� ����
	float CapsuleHeight = ScaledHeight * 1.f;
	physx::PxVec3 DynamicCenter = physx::PxVec3{ 0.0f, CapsuleHeight, 0.0f };
	physx::PxTransform relativePose(physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0, 0, 1)));
	relativePose.p = DynamicCenter;
	m_pShape->setLocalPose(relativePose);

	//�浹�Ҷ� �ʿ��� ���� ������
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
	
	// ����?
	//m_pRigidDynamic->setLinearDamping(physx::PxReal(0.01f));
	//m_pRigidDynamic->setMaxAngularVelocity(physx::PxReal(20.0f));
	//m_pRigidDynamic->setAngularDamping(physx::PxReal(2.0f));

	if (m_pScene == nullptr)
	{
		std::string LevelName = GetLevel()->GetName().data();
		MsgAssert("1. Start���� ���������� �������� ������\n2. ������ CreateScene �ϼ���\n  ������ �� ���� �̸� : " + LevelName);
	}

	// Scene�� ���� �߰�
	m_pScene->addActor(*m_pRigidDynamic);

	// TODO::virtualPhysXLevel���� Callback �Լ��� ȣ���ϱ� ���� Dynamic ����
	return m_pRigidDynamic;
}

void PhysXSphereComponent::SetMoveSpeed(float4 _MoveSpeed)
{
	// RigidDynamic�� ���� �����ϴ� Flag -> ĳ���Ͱ� �������� �ʰ� ������ �� �ֵ���
	// ���𰡿� �浹�ؼ� �������� �Ұ�쿡�� setRigidDynamicLockFlag({flag}, false)�� flag�� �����ؾ���.
	m_pRigidDynamic->setRigidDynamicLockFlags(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y | physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);

	// ĳ������ ������ ������ ����
	m_pRigidDynamic->addForce(physx::PxVec3(_MoveSpeed.x, _MoveSpeed.y, _MoveSpeed.z), physx::PxForceMode::eFORCE);
}


void PhysXSphereComponent::SetDynamicIdle()
{
	// ������ ���� ����
	m_pRigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, false);
	m_pRigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, false);
	m_pRigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, false);

	// Kinematic�� ������� ���, RigidDynamic���� ���ư� �� �ֵ��� Flag����
	//dynamic_->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, false);
}

void PhysXSphereComponent::Start()
{
	// �θ��� ������ ����
	ParentActor = GetActor()->DynamicThis<GameEngineActor>();
}

void PhysXSphereComponent::Update(float _DeltaTime)
{
	if (!(physx::PxIsFinite(m_pRigidDynamic->getGlobalPose().p.x) || physx::PxIsFinite(m_pRigidDynamic->getGlobalPose().p.y) || physx::PxIsFinite(m_pRigidDynamic->getGlobalPose().p.z))
		&& true == IsMain)
	{
		m_pRigidDynamic->setGlobalPose(RecentTransform);
	}

	// PhysX Actor�� ���¿� ���缭 �θ��� Transform������ ����
	float4 tmpWorldPos = { m_pRigidDynamic->getGlobalPose().p.x, m_pRigidDynamic->getGlobalPose().p.y, m_pRigidDynamic->getGlobalPose().p.z };
	float4 EulerRot = PhysXDefault::GetQuaternionEulerAngles(m_pRigidDynamic->getGlobalPose().q) * GameEngineMath::RadToDeg;

	ParentActor.lock()->GetTransform()->SetWorldRotation(float4{ EulerRot.x, EulerRot.y, EulerRot.z });
	ParentActor.lock()->GetTransform()->SetWorldPosition(tmpWorldPos);

	if (m_bSpeedLimit == true)
	{
		SpeedLimit();
	}
}

void PhysXSphereComponent::PushImpulse(float4 _ImpulsePower)
{
	// ������ ���� ����
	m_pRigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, false);
	m_pRigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, false);
	m_pRigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, false);

	m_pRigidDynamic->addForce(physx::PxVec3(_ImpulsePower.x, _ImpulsePower.y, _ImpulsePower.z), physx::PxForceMode::eIMPULSE);
}

void PhysXSphereComponent::PushImpulseAtLocalPos(float4 _ImpulsePower, float4 _Pos)
{
	// ������ ���� ����
	m_pRigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, false);
	m_pRigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, false);

	physx::PxRigidBodyExt::addForceAtPos(*m_pRigidDynamic, physx::PxVec3(_Pos.x, _Pos.y * 0.9f, _Pos.z),
		physx::PxVec3(_ImpulsePower.x, _ImpulsePower.y, _ImpulsePower.z), physx::PxForceMode::eIMPULSE, true);
}

void PhysXSphereComponent::SpeedLimit()
{
	physx::PxVec3 Velo = m_pRigidDynamic->getLinearVelocity();
	physx::PxVec2 Velo2D(Velo.x, Velo.z);

	if (Velo2D.magnitude() > PLAYER_MAX_SPEED)
	{
		Velo2D.normalize();
		Velo2D *= PLAYER_MAX_SPEED;
		Velo.x = Velo2D.x;
		Velo.z = Velo2D.y;

		m_pRigidDynamic->setLinearVelocity(Velo);
	}
}

void PhysXSphereComponent::SetChangedRot(float4 _Rot)
{
	//dynamic_->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, false);

	float4 tmpQuat = _Rot.EulerDegToQuaternion();
	const physx::PxQuat tmpPxQuat(tmpQuat.x, tmpQuat.y, tmpQuat.z, tmpQuat.w);
	const physx::PxTransform tmpTansform(m_pRigidDynamic->getGlobalPose().p, tmpPxQuat);

	m_pRigidDynamic->setGlobalPose(tmpTansform);
}

//�÷��̾� ���ߴ� �Լ�
void PhysXSphereComponent::FreezeDynamic()
{
	m_pRigidDynamic->putToSleep();
}

void PhysXSphereComponent::WakeUpDynamic()
{
	m_pRigidDynamic->wakeUp();
}

void PhysXSphereComponent::ResetDynamic()
{
	float4 tmpQuat = float4{ 0.0f,0.0f,0.0f }.EulerDegToQuaternion();
	const physx::PxQuat tmpPxQuat(tmpQuat.x, tmpQuat.y, tmpQuat.z, tmpQuat.w);
	const physx::PxTransform tmpTansform(m_pRigidDynamic->getGlobalPose().p, tmpPxQuat);

	m_pRigidDynamic->setGlobalPose(tmpTansform);
	m_pRigidDynamic->setRigidDynamicLockFlags(
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X |
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y |
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
}


