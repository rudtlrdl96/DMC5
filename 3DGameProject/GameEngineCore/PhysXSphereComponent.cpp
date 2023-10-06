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
	m_pDynamic = m_pPhysics->createRigidDynamic(localTm);

	// Ư�� ���� ����/������ ������ ��״� ��Ŀ������ �����ϴ� �÷��� ����
	m_pDynamic->setRigidDynamicLockFlags
	(
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X |
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y |
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z
	);

	float ScaledRadius = tmpGeoMetryScale.z;
	float ScaledHeight = tmpGeoMetryScale.y;

	// �浹ü�� ����
	// �浹ü�� ũ��� ������ ũ�⸦ �����ϹǷ� ���� Renderer�� �������� �浹ü�� 2��� �����Ǿ�� ��
	// TODO::�θ� ������ RenderUnit���κ��� Mesh�� Scale �� WorldScale�� ������ ����� ������Ʈ���� Scale�� �����ؾ���.
	m_pShape = physx::PxRigidActorExt::createExclusiveShape(*m_pDynamic, physx::PxSphereGeometry(physx::PxReal(tmpGeoMetryScale.y * 0.5f)), *m_pMaterial);

	// RigidDynamic�� �е��� ����
	physx::PxRigidBodyExt::updateMassAndInertia(*m_pDynamic, 0.01f);

	//�ǹ� ����
	float CapsuleHeight = ScaledHeight * 1.f;
	physx::PxVec3 DynamicCenter = physx::PxVec3{ 0.0f, tmpGeoMetryScale.y, 0.0f };
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
	
	if (m_pScene == nullptr)
	{
		std::string LevelName = GetLevel()->GetName().data();
		MsgAssert("1. Start���� ���������� �������� ������\n2. ������ CreateScene �ϼ���\n  ������ �� ���� �̸� : " + LevelName);
	}

	PhysicsComponent = DynamicThis<PhysXSphereComponent>();
	m_pShape->userData = GetActor();

	GetActor()->SetPhysicsActor();

	// Scene�� ���� �߰�
	m_pScene->addActor(*m_pDynamic);
}

void PhysXSphereComponent::Start()
{
	// �θ��� ������ ����
	ParentActor = GetActor()->DynamicThis<GameEngineActor>();
}

void PhysXSphereComponent::Update(float _DeltaTime)
{
	if (!(physx::PxIsFinite(m_pDynamic->getGlobalPose().p.x) || physx::PxIsFinite(m_pDynamic->getGlobalPose().p.y) || physx::PxIsFinite(m_pDynamic->getGlobalPose().p.z))
		&& true == IsMain)
	{
		m_pDynamic->setGlobalPose(RecentTransform);
	}

	// PhysX Actor�� ���¿� ���缭 �θ��� Transform������ ����
	float4 tmpWorldPos = { m_pDynamic->getGlobalPose().p.x, m_pDynamic->getGlobalPose().p.y, m_pDynamic->getGlobalPose().p.z };
	float4 EulerRot = PhysXDefault::GetQuaternionEulerAngles(m_pDynamic->getGlobalPose().q) * GameEngineMath::RadToDeg;

	ParentActor.lock()->GetTransform()->SetWorldRotation(float4{ EulerRot.x, EulerRot.y, EulerRot.z });
	ParentActor.lock()->GetTransform()->SetWorldPosition(tmpWorldPos);
}

void PhysXSphereComponent::SetWorldPosition(float4 _Value)
{
	physx::PxTransform CurTansform = m_pDynamic->getGlobalPose();

	float ValueX = _Value.x;
	float ValueY = _Value.y;
	float ValueZ = _Value.z;

	CurTansform.p = { _Value.x, _Value.y, _Value.z };

	m_pDynamic->setGlobalPose(CurTansform);
}

void PhysXSphereComponent::AddWorldPosition(float4 _Value)
{
	physx::PxTransform CurTansform = m_pDynamic->getGlobalPose();

	float ValueX = _Value.x;
	float ValueY = _Value.y;
	float ValueZ = _Value.z;

	CurTansform.p += { _Value.x, _Value.y, _Value.z };

	m_pDynamic->setGlobalPose(CurTansform);
}

void PhysXSphereComponent::SetWorldRotation(float4 _Value)
{
	float4 tmpQuat = _Value.DegreeRotationToQuaternionReturn();

	const physx::PxQuat tmpPxQuat(tmpQuat.x, tmpQuat.y, tmpQuat.z, tmpQuat.w);
	const physx::PxTransform tmpTansform(m_pDynamic->getGlobalPose().p, tmpPxQuat);

	m_pDynamic->setGlobalPose(tmpTansform);
}

void PhysXSphereComponent::AddWorldRotation(float4 _Value)
{
	float4 AddRptation = _Value;
	physx::PxQuat CurQuat = m_pDynamic->getGlobalPose().q;
	float4 CurQuatToFloat4 = { CurQuat.x , CurQuat.y, CurQuat.z, CurQuat.w };
	float4 CurRotation = CurQuatToFloat4.QuaternionToEulerDeg();
	float4 ResultRotation = { CurRotation.x + AddRptation.x, CurRotation.y + AddRptation.y, CurRotation.z + AddRptation.z, CurRotation.w + AddRptation.w };

	float4 ResultsQuat = ResultRotation.DegreeRotationToQuaternionReturn();
	const physx::PxQuat tmpPxQuat(ResultsQuat.x, ResultsQuat.y, ResultsQuat.z, ResultsQuat.w);
	const physx::PxTransform tmpTansform(m_pDynamic->getGlobalPose().p, tmpPxQuat);

	m_pDynamic->setGlobalPose(tmpTansform);
}

void PhysXSphereComponent::SetAirState(float _Power)
{
	m_pDynamic->setLinearVelocity({ 0,0,0 });
	m_pDynamic->addForce(physx::PxVec3(0.0f, _Power, 0.0f), physx::PxForceMode::eIMPULSE);
}

void PhysXSphereComponent::SetJump(float _JumpPower)
{
	m_pDynamic->addForce(physx::PxVec3(0.0f, _JumpPower, 0.0f), physx::PxForceMode::eIMPULSE);
}

void PhysXSphereComponent::SetMove(float4 _MoveSpeed)
{
	//Y���� �߷¿� ���� ���ӵ��� ������ X,Z�� ���ӵ��� ���ּ� ���� �̵��� �ϰԲ� ��
	m_pDynamic->setLinearVelocity({ 0,GetLinearVelocity().y,0 });

	// ĳ������ ������ ������ ����
	m_pDynamic->addForce(physx::PxVec3(_MoveSpeed.x, _MoveSpeed.y, _MoveSpeed.z), physx::PxForceMode::eVELOCITY_CHANGE);
}

void PhysXSphereComponent::SetForce(float4 _MoveSpeed)
{
	m_pDynamic->addForce(physx::PxVec3(_MoveSpeed.x, _MoveSpeed.y, _MoveSpeed.z), physx::PxForceMode::eFORCE);
}

void PhysXSphereComponent::SetPush(float4 _Push)
{
	m_pDynamic->addForce(physx::PxVec3(_Push.x, _Push.y, _Push.z), physx::PxForceMode::eIMPULSE);
}

void PhysXSphereComponent::ResetDynamic()
{
	float4 tmpQuat = float4{ 0.0f,0.0f,0.0f }.DegreeRotationToQuaternionReturn();
	const physx::PxQuat tmpPxQuat(tmpQuat.x, tmpQuat.y, tmpQuat.z, tmpQuat.w);
	const physx::PxTransform tmpTansform(m_pDynamic->getGlobalPose().p, tmpPxQuat);

	m_pDynamic->setGlobalPose(tmpTansform);
	m_pDynamic->setRigidDynamicLockFlags
	(
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X |
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y |
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z
	);
}