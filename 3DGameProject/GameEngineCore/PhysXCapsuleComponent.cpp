#include "PrecompileHeader.h"
#include "PhysXCapsuleComponent.h"

#include <GameEngineBase/GameEngineMath.h>

PhysXCapsuleComponent::PhysXCapsuleComponent()
{
}

PhysXCapsuleComponent::~PhysXCapsuleComponent()
{
}

physx::PxRigidDynamic* PhysXCapsuleComponent::CreatePhysXActors(physx::PxScene* _Scene, physx::PxPhysics* _physics, physx::PxVec3 _GeoMetryScale, float4 _GeoMetryRotation)
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
	//m_pShape->setSimulationFilterData(physx::PxFilterData(static_cast<physx::PxU32>(PhysXFilterGroup::PlayerDynamic), static_cast<physx::PxU32>(PhysXFilterGroup::Ground), static_cast<physx::PxU32>(PhysXFilterGroup::Obstacle), 0));

	m_pShape->setContactOffset(0.2f);

	// ����?
	//m_pDynamic->setLinearDamping(physx::PxReal(0.01f));
	//m_pDynamic->setMaxAngularVelocity(physx::PxReal(20.0f));
	//m_pDynamic->setAngularDamping(physx::PxReal(2.0f));

	// Scene�� ���� �߰�
	_Scene->addActor(*m_pDynamic);

	// TODO::virtualPhysXLevel���� Callback �Լ��� ȣ���ϱ� ���� Dynamic ����
	return m_pDynamic;
}

void PhysXCapsuleComponent::SetMoveSpeed(float4 _MoveSpeed)
{
	// RigidDynamic�� ���� �����ϴ� Flag -> ĳ���Ͱ� �������� �ʰ� ������ �� �ֵ���
	// ���𰡿� �浹�ؼ� �������� �Ұ�쿡�� setRigidDynamicLockFlag({flag}, false)�� flag�� �����ؾ���.
	m_pDynamic->setRigidDynamicLockFlags
	(
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | 
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y |
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z
	);

	// ĳ������ ������ ������ ����
	m_pDynamic->addForce(physx::PxVec3(_MoveSpeed.x, _MoveSpeed.y, _MoveSpeed.z), physx::PxForceMode::eFORCE);
}

void PhysXCapsuleComponent::SetMoveJump()
{
	m_pDynamic->addForce(physx::PxVec3(0.0f, PLAYER_JUMP_FORCE, 0.0f), physx::PxForceMode::eIMPULSE);
}

void PhysXCapsuleComponent::SetMoveDive(float _Rot)
{
	float4 DirVec = float4::DegreeToDirection2D(_Rot);
	DirVec *= 1.0f;
	m_pDynamic->addForce(physx::PxVec3(DirVec.y, PLAYER_JUMP_FORCE * 0.5f, DirVec.x), physx::PxForceMode::eIMPULSE);
}

void PhysXCapsuleComponent::SetDynamicIdle()
{
	// ������ ���� ����
	m_pDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, false);
	m_pDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, false);
	m_pDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, false);

	// Kinematic�� ������� ���, RigidDynamic���� ���ư� �� �ֵ��� Flag����
	//dynamic_->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, false);
}

void PhysXCapsuleComponent::Start()
{
	// �θ��� ������ ����
	ParentActor = GetActor()->DynamicThis<GameEngineActor>();
}

void PhysXCapsuleComponent::Update(float _DeltaTime)
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

	if (IsSpeedLimit == true)
	{
		SpeedLimit();
	}
}

void PhysXCapsuleComponent::PushImpulse(float4 _ImpulsePower)
{
	// ������ ���� ����
	m_pDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, false);
	m_pDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, false);
	m_pDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, false);

	m_pDynamic->addForce(physx::PxVec3(_ImpulsePower.x, _ImpulsePower.y, _ImpulsePower.z), physx::PxForceMode::eIMPULSE);
}

void PhysXCapsuleComponent::PushImpulseAtLocalPos(float4 _ImpulsePower, float4 _Pos)
{
	// ������ ���� ����
	m_pDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, false);
	m_pDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, false);

	physx::PxRigidBodyExt::addForceAtPos(*m_pDynamic, physx::PxVec3(_Pos.x, _Pos.y * 0.9f, _Pos.z),
		physx::PxVec3(_ImpulsePower.x, _ImpulsePower.y, _ImpulsePower.z), physx::PxForceMode::eIMPULSE, true);
}

void PhysXCapsuleComponent::SetPlayerStartPos(float4 _Pos)
{
	physx::PxTransform tmpPxTransform(_Pos.x, _Pos.y, _Pos.z);

	// �θ��� Transform������ �������� PhysX Actor�� Ʈ�������� ����
	m_pDynamic->setGlobalPose(tmpPxTransform);
	RecentTransform = tmpPxTransform;
}

bool PhysXCapsuleComponent::PlayerStandUp(float _DeltaTime, bool _IsXAixisRotReady)
{
	bool Result = false;

	physx::PxVec3 GlobalPose(m_pDynamic->getGlobalPose().p);
	physx::PxVec3 YAxis(0.0f, 1.0f, 0.0f);
	//float YAixsGlobalAngle = std::acosf();
	physx::PxQuat GlobalRot = m_pDynamic->getGlobalPose().q;
	float4 GlobalRotEuler = PhysXDefault::GetQuaternionEulerAngles(GlobalRot);
	float4 AddedRotX = float4{ -1.0f, 0.0f, 0.0f }.DegreeRotationToQuaternionReturn();
	float4 AddedRotY = float4{ 0.0f, 10.0f, 0.0f }.DegreeRotationToQuaternionReturn();
	float4 AddedRotmY = float4{ 0.0f, -5.0f, 0.0f }.DegreeRotationToQuaternionReturn();
	float4 AddedRot = float4{ 0.0f, 0.0f, 0.0f }.DegreeRotationToQuaternionReturn();
	physx::PxQuat FinalRot;
	physx::PxQuat AddedRotQuat(AddedRotY.x, AddedRotY.y, AddedRotY.z, AddedRotY.w);
	FinalRot = GlobalRot * AddedRotQuat;

	//if (GlobalRotEuler.y * GameEngineMath::RadianToDegree >= 90.0f)
	//{
	//	physx::PxQuat AddedRotQuat(AddedRotY.x, AddedRotY.y, AddedRotY.z, AddedRotY.w);
	//	FinalRot = GlobalRot * AddedRotQuat;
	//}
	//else
	//{
	//	physx::PxQuat AddedRotQuat(AddedRotmY.x, AddedRotmY.y, AddedRotmY.z, AddedRotmY.w);
	//	FinalRot = GlobalRot * AddedRotQuat;
	//}

	physx::PxTransform FinalTransform(m_pDynamic->getGlobalPose().p, FinalRot);
	float4 EulerFinalRot = PhysXDefault::GetQuaternionEulerAngles(FinalRot);
	std::string Asdf = std::to_string(EulerFinalRot.x * GameEngineMath::RadToDeg) + ", " +
		std::to_string(EulerFinalRot.y * GameEngineMath::RadToDeg) + ", "
		+ std::to_string(EulerFinalRot.z * GameEngineMath::RadToDeg) + "\n";
	//OutputDebugString(Asdf.c_str());

	m_pDynamic->setGlobalPose(FinalTransform);
	physx::PxQuat QautPose = m_pDynamic->getGlobalPose().q;
	float4 EulerPose = PhysXDefault::GetQuaternionEulerAngles(QautPose);
	float4 EulerPoseDegree = EulerPose * GameEngineMath::RadToDeg;
	//physx::PxQuat newLocalRotation = dynamic_->getGlobalPose().q * Quaternion.Inverse(transform.parent.rotation)
	return Result;
}

bool PhysXCapsuleComponent::StandUp2(float _DeltaTime, bool _IsXAixisRotReady)
{
	bool Result = false;
	if (_IsXAixisRotReady == false)
	{
		return PlayerStandUp(_DeltaTime, _IsXAixisRotReady);
	}

	physx::PxQuat QautPose = m_pDynamic->getGlobalPose().q;

	float4 EulerPose = PhysXDefault::GetQuaternionEulerAngles(QautPose);
	float4 EulerPoseDegree = EulerPose * GameEngineMath::RadToDeg;
	// dynamic�� Angle, Axis�� ���Ѵ�
	float Angle;
	physx::PxVec3 Vec3;
	m_pDynamic->getGlobalPose().q.toRadiansAndUnitAxis(Angle, Vec3);
	//������ �Ǵ� Y-Axis  ����
	float4 XAxis(1.0f, 0.0f, 0.0f);
	float4 YAxis(0.0f, 1.0f, 0.0f);
	float4 ZAxis(0.0f, 0.0f, 1.0f);
	physx::PxVec3 YAxisVec3(0.0f, 1.0f, 0.0f);
	physx::PxVec3 mYAxisVec3(0.0f, -1.0f, 0.0f);
	float AngleDegree = Angle * GameEngineMath::RadToDeg;

	physx::PxVec3 ASDASFFASGG = m_pDynamic->getGlobalPose().q.getBasisVector1();
	float4 XZAngle = float4{ ASDASFFASGG.x, 0.0f, ASDASFFASGG.z };
	XZAngle.Normalize();
	float AngDiffXZ = atanf(XZAngle.x / XZAngle.z);

	if (ASDASFFASGG.x < 0.0f && ASDASFFASGG.z < 0.0f)
	{
		AngDiffXZ += GameEngineMath::PIE;
	}


	if (ASDASFFASGG.x > 0.0f && ASDASFFASGG.z < 0.0f)
	{
		AngDiffXZ -= GameEngineMath::PIE;
	}

	if (AngDiffXZ < 0.0f)
	{
		AngDiffXZ += GameEngineMath::PIE * 2.0f;
	}

	//dynamic�� Axis�� Y-Axis ������ ����

	float AngDiff = acosf(float4::DotProduct3D({ Vec3.x, Vec3.y, Vec3.z }, YAxis));
	float AngDiffEuler = AngDiff * GameEngineMath::RadToDeg;
	//if (AngDiffEuler);
	//dynamic�� Axis�� Y-Axis ������ NoramlVector
	physx::PxVec3 Normal = Vec3.cross(YAxisVec3);
	physx::PxVec3 mNormal = Vec3.cross(mYAxisVec3);
	Normal.normalize();
	StandUpProgressYAxisAngle += _DeltaTime;
	float ChangedAngle = GameEngineMath::LerpLimit(StandUpStartYAxisAngle, StandUpTargetYAxisAngle, StandUpProgressYAxisAngle * 3.0f);

	//�븻���͸� �������� YAxis�� AngDiff��ŭ ȸ��
	float4 FinalRot;
	GameEngineDebug::OutPutString(std::to_string(Vec3.x) + ", " + std::to_string(Vec3.y) + ", " + std::to_string(Vec3.z));
	//if (Vec3.y > 0)
	//{
	FinalRot = RodriguesRotate({ Vec3.x, Vec3.y, Vec3.z }, { Normal.x, Normal.y, Normal.z }, 0.08f);
	//}
	//else
	//{
	//	FinalRot = RodriguesRotate({ Vec3.x, Vec3.y, Vec3.z }, { Normal.x, Normal.y, Normal.z }, -0.08f);
	//}


	if (abs(FinalRot.y) > 0.97f)
	{
		FinalRot.x = 0.0f;
		FinalRot.z = 0.0f;
		FinalRot.y = 1.0f;
		Result = true;
		GameEngineDebug::OutPutString("WakeUp");
	}

	//if (abs(FinalRot.x) > 0.97f && ChangedAngle == 0.0f)
	//{
	//	//float AngDiffXAxis = acosf(float4::DotProduct3D({ Vec3.x, Vec3.y, Vec3.z }, XAxis));
	//	FinalRot.x = 0.0f;
	//	FinalRot.z = 0.0f;
	//	FinalRot.y = 1.0f;
	//	Result = true;
	//	GameEngineDebug::OutPutString("WakeUp");

	//}

	physx::PxVec3 FinalRotVec3(FinalRot.x, FinalRot.y, FinalRot.z);
	FinalRotVec3.normalize();
	physx::PxQuat tmpQuat(ChangedAngle, FinalRotVec3);
	//const physx::PxQuat tmpPxQuat(tmpQuat.x, tmpQuat.y, tmpQuat.z, tmpQuat.w);
	const physx::PxTransform tmpTransform(m_pDynamic->getGlobalPose().p, tmpQuat);
	m_pDynamic->setGlobalPose(tmpTransform);

	return Result;

}

void PhysXCapsuleComponent::SpeedLimit()
{
	physx::PxVec3 Velo = m_pDynamic->getLinearVelocity();
	physx::PxVec2 Velo2D(Velo.x, Velo.z);

	if (Velo2D.magnitude() > PLAYER_MAX_SPEED)
	{
		Velo2D.normalize();
		Velo2D *= PLAYER_MAX_SPEED;
		Velo.x = Velo2D.x;
		Velo.z = Velo2D.y;

		m_pDynamic->setLinearVelocity(Velo);
	}
}

void PhysXCapsuleComponent::SetChangedRot(float4 _Rot)
{
	//dynamic_->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, false);

	float4 tmpQuat = _Rot.DegreeRotationToQuaternionReturn();
	const physx::PxQuat tmpPxQuat(tmpQuat.x, tmpQuat.y, tmpQuat.z, tmpQuat.w);
	const physx::PxTransform tmpTansform(m_pDynamic->getGlobalPose().p, tmpPxQuat);

	m_pDynamic->setGlobalPose(tmpTansform);
}

//�÷��̾� ���ߴ� �Լ�
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
	m_pDynamic->setRigidDynamicLockFlags(
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X |
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y |
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
}

void PhysXCapsuleComponent::InitializeStandUp2()
{
	physx::PxVec3 ASDASFFASGG = m_pDynamic->getGlobalPose().q.getBasisVector1();
	float4 XZAngle = float4{ ASDASFFASGG.x, 0.0f, ASDASFFASGG.z };
	XZAngle.Normalize();
	float AngDiffXZ = atanf(XZAngle.x / XZAngle.z);

	if (ASDASFFASGG.x < 0.0f && ASDASFFASGG.z < 0.0f)
	{
		AngDiffXZ += GameEngineMath::PIE;
	}

	if (ASDASFFASGG.x > 0.0f && ASDASFFASGG.z < 0.0f)
	{
		AngDiffXZ -= GameEngineMath::PIE;
	}

	if (AngDiffXZ < 0.0f)
	{
		AngDiffXZ += GameEngineMath::PIE * 2.0f;
	}

	StandUpTargetYAxisAngle = AngDiffXZ;

	float Angle;
	physx::PxVec3 Vec3;
	m_pDynamic->getGlobalPose().q.toRadiansAndUnitAxis(Angle, Vec3);

	StandUpStartYAxisAngle = Angle;

	StandUpProgressYAxisAngle = 0.0f;
}

