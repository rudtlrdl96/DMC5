#include "PrecompileHeader.h"
#include "PhysXCapsuleComponent.h"

#include <GameEngineBase/GameEngineMath.h>

PhysXCapsuleComponent::PhysXCapsuleComponent()
{
}

PhysXCapsuleComponent::~PhysXCapsuleComponent()
{
}

void PhysXCapsuleComponent::Start()
{
	// �θ��� ������ ����
	ParentActor = GetActor()->DynamicThis<GameEngineActor>();
}

void PhysXCapsuleComponent::Update(float _DeltaTime)
{
	SpeedLimit();

	if (!(physx::PxIsFinite(m_pDynamic->getGlobalPose().p.x) || physx::PxIsFinite(m_pDynamic->getGlobalPose().p.y) || physx::PxIsFinite(m_pDynamic->getGlobalPose().p.z)))
	{
		m_pDynamic->setGlobalPose(RecentTransform);
	}

	// PhysX Actor�� ���¿� ���缭 �θ��� Transform������ ����
	float4 tmpWorldPos = { m_pDynamic->getGlobalPose().p.x, m_pDynamic->getGlobalPose().p.y, m_pDynamic->getGlobalPose().p.z };
	float4 EulerRot = PhysXDefault::GetQuaternionEulerAngles(m_pDynamic->getGlobalPose().q) * GameEngineMath::RadToDeg;

	ParentActor.lock()->GetTransform()->SetWorldRotation(float4{ EulerRot.x, EulerRot.y, EulerRot.z });
	ParentActor.lock()->GetTransform()->SetWorldPosition(tmpWorldPos);
}

void PhysXCapsuleComponent::CreatePhysXActors(physx::PxScene* _Scene, physx::PxPhysics* _physics, physx::PxVec3 _GeoMetryScale, float4 _GeoMetryRotation)
{
	m_pScene = _Scene;
	m_pPhysics = _physics;

	float4 tmpQuat = _GeoMetryRotation.DegreeRotationToQuaternionReturn();

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
	//physx::PxVec3 tmpGeoMetryScale
	// (
	//		_GeoMetryScale.x * tmpMagnification.x * 0.5f, 
	//		_GeoMetryScale.y * tmpMagnification.y * 0.5f, 
	//		_GeoMetryScale.z * tmpMagnification.z * 0.5f
	// );

	//GeoMetryScale = _GeoMetryScale;

	physx::PxVec3 tmpGeoMetryScale
	(
		_GeoMetryScale.x * 0.5f,
		_GeoMetryScale.y * 0.5f,
		_GeoMetryScale.z * 0.5f
	);

	// �浹ü�� ����
	m_pDynamic = _physics->createRigidDynamic(localTm);

	// Ư�� ���� ����/������ ������ ��״� ��Ŀ������ �����ϴ� �÷��� ����
	// ���𰡿� �浹�ؼ� �������� �Ұ�쿡�� setRigidDynamicLockFlag({flag}, false)�� flag�� �����ؾ���.
	m_pDynamic->setRigidDynamicLockFlags
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
	m_pShape = physx::PxRigidActorExt::createExclusiveShape(*m_pDynamic, physx::PxCapsuleGeometry(ScaledRadius * 1.f, ScaledHeight * 1.f), *m_pMaterial);

	// RigidDynamic�� �е��� ����
	physx::PxRigidBodyExt::updateMassAndInertia(*m_pDynamic, 0.01f);

	//�ǹ� ����
	float CapsuleHeight = ScaledHeight * 1.f;
	physx::PxVec3 DynamicCenter = physx::PxVec3{ 0.0f, CapsuleHeight, 0.0f };
	physx::PxTransform relativePose(physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0, 0, 1)));
	relativePose.p = DynamicCenter;
	m_pShape->setLocalPose(relativePose);

	//�浹�Ҷ� �ʿ��� ���� ������
	//m_pShape->setSimulationFilterData
	//(
	//	physx::PxFilterData
	//	(
	//		static_cast<physx::PxU32>(PhysXFilterGroup::Obstacle),
	//		static_cast<physx::PxU32>(PhysXFilterGroup::PlayerDynamic), 
	//		0,
	//		0
	//	)
	//);
	m_pShape->setSimulationFilterData(physx::PxFilterData(static_cast<physx::PxU32>(PhysXFilterGroup::PlayerDynamic), static_cast<physx::PxU32>(PhysXFilterGroup::Ground), static_cast<physx::PxU32>(PhysXFilterGroup::Obstacle), 0));

	m_pShape->setContactOffset(0.2f);

	//RayCastTarget False;
	m_pShape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, false);

	// ����?
	//m_pDynamic->setLinearDamping(physx::PxReal(0.01f));
	//m_pDynamic->setMaxAngularVelocity(physx::PxReal(20.0f));
	//m_pDynamic->setAngularDamping(physx::PxReal(2.0f));

	// Scene�� ���� �߰�
	_Scene->addActor(*m_pDynamic);
}

//void Collider::SetRayCastTarget(const bool _bRayCastTarget)
//{
//	if (_bRayCastTarget == m_bRayCastTarget)
//		return;
//	if (nullptr == m_pShape)
//		return;
//
//	m_bRayCastTarget = _bRayCastTarget;
//
//	m_pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, _bRayCastTarget);
//}

void PhysXCapsuleComponent::SetWorldPosition(float4 _Value)
{
	physx::PxTransform CurTansform = m_pDynamic->getGlobalPose();

	float ValueX = _Value.x;
	float ValueY = _Value.y;
	float ValueZ = _Value.z;

	CurTansform.p += {_Value.x, _Value.y, _Value.z};

	m_pDynamic->setGlobalPose(CurTansform);
}

void PhysXCapsuleComponent::SetWorldRotation(float4 _Value)
{
	float4 tmpQuat = _Value.DegreeRotationToQuaternionReturn();

	const physx::PxQuat tmpPxQuat(tmpQuat.x, tmpQuat.y, tmpQuat.z, tmpQuat.w);
	const physx::PxTransform tmpTansform(m_pDynamic->getGlobalPose().p, tmpPxQuat);

	m_pDynamic->setGlobalPose(tmpTansform);
}

void PhysXCapsuleComponent::SpeedLimit()
{
	physx::PxVec3 Velo = m_pDynamic->getLinearVelocity();
	physx::PxVec2 Velo2D(Velo.x, Velo.z);

	if (Velo2D.magnitude() > SpeedLimitValue)
	{
		Velo2D.normalize();
		Velo2D *= SpeedLimitValue;
		Velo.x = Velo2D.x;
		Velo.z = Velo2D.y;

		m_pDynamic->setLinearVelocity(Velo);
	}
}

void PhysXCapsuleComponent::SetJump(float _JumpPower)
{
	m_pDynamic->addForce(physx::PxVec3(0.0f, _JumpPower, 0.0f), physx::PxForceMode::eIMPULSE);
}

void PhysXCapsuleComponent::SetMove(float4 _MoveSpeed)
{
	//m_pDynamic->setLinearVelocity({ 0,-GetLinearVelocity().y,0 });
	m_pDynamic->addForce(physx::PxVec3(_MoveSpeed.x, _MoveSpeed.y, _MoveSpeed.z), physx::PxForceMode::eVELOCITY_CHANGE);
}

void PhysXCapsuleComponent::SetForce(float4 _MoveSpeed)
{
	m_pDynamic->addForce(physx::PxVec3(_MoveSpeed.x, _MoveSpeed.y, _MoveSpeed.z), physx::PxForceMode::eFORCE);
}

void PhysXCapsuleComponent::SetPush(float4 _Push)
{
	m_pDynamic->addForce(physx::PxVec3(_Push.x, _Push.y, _Push.z), physx::PxForceMode::eIMPULSE);
}

void PhysXCapsuleComponent::PushImpulseAtLocalPos(float4 _ImpulsePower, float4 _Pos)
{
	physx::PxRigidBodyExt::addForceAtPos
	(
		*m_pDynamic, 
		physx::PxVec3
		(
			_Pos.x, 
			_Pos.y * 0.9f,
			_Pos.z
		),
		physx::PxVec3
		(
			_ImpulsePower.x, 
			_ImpulsePower.y, 
			_ImpulsePower.z
		), 
		physx::PxForceMode::eIMPULSE, 
		true
	);
}

void PhysXCapsuleComponent::SetPlayerStartPos(float4 _Pos)
{
	physx::PxTransform tmpPxTransform(_Pos.x, _Pos.y, _Pos.z);

	// �θ��� Transform������ �������� PhysX Actor�� Ʈ�������� ����
	m_pDynamic->setGlobalPose(tmpPxTransform);
	RecentTransform = tmpPxTransform;
}

void PhysXCapsuleComponent::FreezeDynamic()
{
	m_pDynamic->putToSleep();
}

void PhysXCapsuleComponent::WakeUpDynamic()
{
	m_pDynamic->wakeUp();
}

void PhysXCapsuleComponent::ResetDynamic()
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