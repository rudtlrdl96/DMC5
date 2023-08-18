#include "PrecompileHeader.h"
#include "PhysXDynamicActorComponent.h"

#include <GameEngineBase/GameEngineMath.h>

PhysXDynamicActorComponent::PhysXDynamicActorComponent()
{
}

PhysXDynamicActorComponent::~PhysXDynamicActorComponent()
{
}

physx::PxRigidDynamic* PhysXDynamicActorComponent::CreatePhysXActors(physx::PxScene* _Scene, physx::PxPhysics* _physics, physx::PxVec3 _GeoMetryScale, float4 _GeoMetryRot)
{
	m_pScene = _Scene;
	m_pPhysics = _physics;

	float4 tmpQuat = _GeoMetryRot.DegreeRotationToQuaternionReturn();

	// 부모 액터로부터 위치 생성
	physx::PxTransform localTm(physx::PxVec3
	(
		ParentActor.lock()->GetTransform()->GetWorldPosition().x
		, ParentActor.lock()->GetTransform()->GetWorldPosition().y
		, ParentActor.lock()->GetTransform()->GetWorldPosition().z
	),
		physx::PxQuat(tmpQuat.x, tmpQuat.y, tmpQuat.z, tmpQuat.w));

	// 마찰, 탄성계수
	m_pMaterial = _physics->createMaterial(Staticfriction, Dynamicfriction, Resitution);

	// TODO::배율을 적용할 경우 이쪽 코드를 사용
	//float4 tmpMagnification = { SIZE_MAGNIFICATION_RATIO };
	//physx::PxVec3 tmpGeoMetryScale(_GeoMetryScale.x * tmpMagnification.x * 0.5f, 
	//							   _GeoMetryScale.y * tmpMagnification.y * 0.5f, 
	//							   _GeoMetryScale.z * tmpMagnification.z * 0.5f);

	GeoMetryScale = _GeoMetryScale;

	physx::PxVec3 tmpGeoMetryScale
	(
		_GeoMetryScale.x * 0.5f,
		_GeoMetryScale.y * 0.5f,
		_GeoMetryScale.z * 0.5f
	);

	//// 충돌체의 종류
	m_pDynamic = _physics->createRigidDynamic(localTm);

	// TODO::점프속력에 영향을 미침. 스테이지 작성후 자세한 수치는 나중에 조절
	// 플레이어 최대 속력
	// dynamic_->setMaxLinearVelocity(PLAYER_MAX_SPEED);

	m_pDynamic->setRigidDynamicLockFlags
	(
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X |
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y |
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z
	);

	float ScaledRadius = _GeoMetryScale.z;
	float ScaledHeight = _GeoMetryScale.y;

	// 메인 캡슐 콜라이더
	m_pShape = physx::PxRigidActorExt::createExclusiveShape(*m_pDynamic, physx::PxCapsuleGeometry(ScaledRadius * 1.3f, ScaledHeight * 0.9f), *m_pMaterial);
	float CapsuleHeight = (ScaledHeight * 0.9f);
	physx::PxVec3 DynamicCenter(0.0f, CapsuleHeight, 0.0f);
	//피벗설정
	physx::PxTransform relativePose(physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0, 0, 1)));
	relativePose.p = DynamicCenter;
	physx::PxTransform relativePose2(DynamicCenter);
	m_pShape->setLocalPose(relativePose);
	//shape_->setSimulationFilterData(physx::PxFilterData(static_cast<physx::PxU32>(PhysXFilterGroup::PlayerDynamic), 
	//	static_cast<physx::PxU32>(PhysXFilterGroup::Ground), 
	//	static_cast<physx::PxU32>(PhysXFilterGroup::Obstacle), 0));
	//physx::PxTransform relativePose(physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0, 0, 1)));
	m_pShape->setContactOffset(0.2f);
	physx::PxRigidBodyExt::updateMassAndInertia(*m_pDynamic, 0.01f);

	m_pInstshape = physx::PxRigidActorExt::createExclusiveShape(*m_pDynamic, physx::PxCapsuleGeometry(ScaledRadius * 1.3f, ScaledHeight * 0.9f), *m_pMaterial);
	// 충돌시점 콜백을 위한 세팅
	//Instshape_->setSimulationFilterData(physx::PxFilterData(static_cast<physx::PxU32>(PhysXFilterGroup::Player), 0, 0, 0));
	m_pInstshape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
	m_pInstshape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);

	//콜백피벗설정 
	m_pInstshape->setLocalPose(relativePose);
	m_pInstshape->setContactOffset(0.2f);
	// 
	physx::PxTransform LocalPose = m_pDynamic->getCMassLocalPose();

	//m_pFaceshape = physx::PxRigidActorExt::createExclusiveShape(*m_pDynamic, physx::PxBoxGeometry(ScaledRadius * 0.1f, ScaledHeight * 0.4f, ScaledHeight * 0.9f), *m_pMaterial);
	//m_pFaceshape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
	//m_pFaceshape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
	//physx::PxTransform facerelativePose(physx::PxVec3(0.0f, CapsuleHeight * 1.3f, ScaledRadius * 1.3f));
	//physx::PxTransform facerelativePose2(facerelativePose);
	//m_pFaceshape->setLocalPose(facerelativePose2);
	////faceshape_->setSimulationFilterData(physx::PxFilterData(static_cast<physx::PxU32>(PhysXFilterGroup::PlayerFace), 0, 0, 0));

	//m_pHeadshape = physx::PxRigidActorExt::createExclusiveShape(*m_pDynamic, physx::PxSphereGeometry(ScaledHeight * 0.9f), *m_pMaterial);
	//m_pHeadshape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
	//m_pHeadshape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
	//physx::PxTransform headrelativePose(physx::PxVec3(0.0f, CapsuleHeight * 1.5f, 0.0f));
	////physx::PxTransform facerelativePose2(headrelativePose);
	//m_pHeadshape->setLocalPose(headrelativePose);
	////headshape_->setSimulationFilterData(physx::PxFilterData(static_cast<physx::PxU32>(PhysXFilterGroup::PlayerHead), 0, 0, 0));
	//m_pHeadshape->setContactOffset(0.2f);

	// 제동?
	m_pDynamic->setLinearDamping(physx::PxReal(0.5f));
	m_pDynamic->setMaxAngularVelocity(physx::PxReal(20.0f));
	m_pDynamic->setAngularDamping(physx::PxReal(2.0f));


	// Scene에 액터 추가
	_Scene->addActor(*m_pDynamic);

	// TODO::virtualPhysXLevel에서 Callback 함수에 호출하기 위한 Dynamic 리턴
	return m_pDynamic;
}

void PhysXDynamicActorComponent::SetMoveSpeed(float4 _MoveSpeed)
{
	// RigidDynamic의 축을 고정하는 Flag -> 캐릭터가 쓰러지지 않고 서있을 수 있도록
	// 무언가와 충돌해서 쓰러져야 할경우에는 setRigidDynamicLockFlag({flag}, false)로 flag를 해제해야함.
	m_pDynamic->setRigidDynamicLockFlags(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y | physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);

	// 캐릭터의 방향을 힘으로 조절
	m_pDynamic->addForce(physx::PxVec3(_MoveSpeed.x, _MoveSpeed.y, _MoveSpeed.z), physx::PxForceMode::eFORCE);
}

void PhysXDynamicActorComponent::SetMoveJump()
{
	m_pDynamic->addForce(physx::PxVec3(0.0f, PLAYER_JUMP_FORCE, 0.0f), physx::PxForceMode::eIMPULSE);
}

void PhysXDynamicActorComponent::SetMoveDive(float _Rot)
{
	float4 DirVec = float4::DegreeToDirection2D(_Rot);
	DirVec *= 1.0f;
	m_pDynamic->addForce(physx::PxVec3(DirVec.y, PLAYER_JUMP_FORCE * 0.5f, DirVec.x), physx::PxForceMode::eIMPULSE);
}

void PhysXDynamicActorComponent::SetDynamicIdle()
{
	// 고정된 축을 해제
	m_pDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, false);
	m_pDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, false);
	m_pDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, false);

	// Kinematic을 사용했을 경우, RigidDynamic으로 돌아갈 수 있도록 Flag해제
	//dynamic_->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, false);
}

void PhysXDynamicActorComponent::Start()
{
	// 부모의 정보의 저장
	ParentActor = GetActor()->DynamicThis<GameEngineActor>();
}

void PhysXDynamicActorComponent::Update(float _DeltaTime)
{
	if (!(physx::PxIsFinite(m_pDynamic->getGlobalPose().p.x)
		|| physx::PxIsFinite(m_pDynamic->getGlobalPose().p.y)
		|| physx::PxIsFinite(m_pDynamic->getGlobalPose().p.z))
		&& true == IsMain)
	{
		m_pDynamic->setGlobalPose(RecentTransform);
	}

	// PhysX Actor의 상태에 맞춰서 부모의 Transform정보를 갱신
	float4 tmpWorldPos = 
	{ 
		m_pDynamic->getGlobalPose().p.x,
		m_pDynamic->getGlobalPose().p.y,
		m_pDynamic->getGlobalPose().p.z 
	};

	//float4 QuatRot = float4{ dynamic_->getGlobalPose().q.x, dynamic_->getGlobalPose().q.y, dynamic_->getGlobalPose().q.z, dynamic_->getGlobalPose().q.w };
	float4 EulerRot = PhysXDefault::GetQuaternionEulerAngles(m_pDynamic->getGlobalPose().q) * GameEngineMath::RadToDeg;

	ParentActor.lock()->GetTransform()->SetWorldRotation(float4{ EulerRot.x, EulerRot.y, EulerRot.z });

	ParentActor.lock()->GetTransform()->SetWorldPosition(tmpWorldPos);

	if (IsSpeedLimit == true)
	{
		SpeedLimit();
	}
}

void PhysXDynamicActorComponent::PushImpulse(float4 _ImpulsePower)
{
	// 고정된 축을 해제
	m_pDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, false);
	m_pDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, false);
	m_pDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, false);

	m_pDynamic->addForce(physx::PxVec3(_ImpulsePower.x, _ImpulsePower.y, _ImpulsePower.z), physx::PxForceMode::eIMPULSE);
}

void PhysXDynamicActorComponent::PushImpulseAtLocalPos(float4 _ImpulsePower, float4 _Pos)
{
	// 고정된 축을 해제
	m_pDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, false);
	m_pDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, false);

	physx::PxRigidBodyExt::addForceAtPos(*m_pDynamic, physx::PxVec3(_Pos.x, _Pos.y * 0.9f, _Pos.z),
		physx::PxVec3(_ImpulsePower.x, _ImpulsePower.y, _ImpulsePower.z), physx::PxForceMode::eIMPULSE, true);
}

void PhysXDynamicActorComponent::SetPlayerStartPos(float4 _Pos)
{
	physx::PxTransform tmpPxTransform(_Pos.x, _Pos.y, _Pos.z);

	// 부모의 Transform정보를 바탕으로 PhysX Actor의 트랜스폼을 갱신
	m_pDynamic->setGlobalPose(tmpPxTransform);
	RecentTransform = tmpPxTransform;
}

bool PhysXDynamicActorComponent::PlayerStandUp(float _DeltaTime, bool _IsXAixisRotReady)
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

bool PhysXDynamicActorComponent::StandUp2(float _DeltaTime, bool _IsXAixisRotReady)
{
	bool Result = false;
	if (_IsXAixisRotReady == false)
	{
		return PlayerStandUp(_DeltaTime, _IsXAixisRotReady);
	}

	physx::PxQuat QautPose = m_pDynamic->getGlobalPose().q;

	float4 EulerPose = PhysXDefault::GetQuaternionEulerAngles(QautPose);
	float4 EulerPoseDegree = EulerPose * GameEngineMath::RadToDeg;
	// dynamic의 Angle, Axis를 구한다
	float Angle;
	physx::PxVec3 Vec3;
	m_pDynamic->getGlobalPose().q.toRadiansAndUnitAxis(Angle, Vec3);
	//기준이 되는 Y-Axis  선언
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

	//dynamic의 Axis와 Y-Axis 사이의 각도

	float AngDiff = acosf(float4::DotProduct3D({ Vec3.x, Vec3.y, Vec3.z }, YAxis));
	float AngDiffEuler = AngDiff * GameEngineMath::RadToDeg;
	//if (AngDiffEuler);
	//dynamic의 Axis와 Y-Axis 사이의 NoramlVector
	physx::PxVec3 Normal = Vec3.cross(YAxisVec3);
	physx::PxVec3 mNormal = Vec3.cross(mYAxisVec3);
	Normal.normalize();
	StandUpProgressYAxisAngle += _DeltaTime;
	float ChangedAngle = GameEngineMath::LerpLimit(StandUpStartYAxisAngle, StandUpTargetYAxisAngle, StandUpProgressYAxisAngle * 3.0f);

	//노말백터를 기준으로 YAxis로 AngDiff만큼 회전
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

void PhysXDynamicActorComponent::SpeedLimit()
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

void PhysXDynamicActorComponent::SetChangedRot(float4 _Rot)
{
	//dynamic_->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, false);

	float4 tmpQuat = _Rot.DegreeRotationToQuaternionReturn();
	const physx::PxQuat tmpPxQuat(tmpQuat.x, tmpQuat.y, tmpQuat.z, tmpQuat.w);
	const physx::PxTransform tmpTansform(m_pDynamic->getGlobalPose().p, tmpPxQuat);

	m_pDynamic->setGlobalPose(tmpTansform);
}

//플레이어 멈추는 함수
void PhysXDynamicActorComponent::FreezeDynamic()
{
	m_pDynamic->putToSleep();
}

void PhysXDynamicActorComponent::WakeUpDynamic()
{
	m_pDynamic->wakeUp();
}

void PhysXDynamicActorComponent::ResetDynamic()
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

void PhysXDynamicActorComponent::InitializeStandUp2()
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

