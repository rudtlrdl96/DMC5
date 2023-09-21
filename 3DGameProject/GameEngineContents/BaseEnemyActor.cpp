#include "PrecompileHeader.h"
#include "BaseEnemyActor.h"

#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>

#include "ContentsEnum.h"
#include "AttackCollision.h"
#include "BasePlayerActor.h"
#include "NetworkManager.h"
#include "FsmChangePacket.h"

BaseEnemyActor::BaseEnemyActor()
{
}

BaseEnemyActor::~BaseEnemyActor()
{
}


void BaseEnemyActor::Update_SendPacket(float _DeltaTime)
{
	NetworkManager::PushUpdatePacket({ .ObjPtr = this, .TimeScale = 1.0f, .UnionData = {0, } });
}

void BaseEnemyActor::Start()
{
	if (false == GameEngineInput::IsKey("MonsterTest"))
	{
		GameEngineInput::CreateKey("MonsterTest", 'M');
	}

	//Render생성
	EnemyRenderer = CreateComponent<GameEngineFBXRenderer>();
	EnemyRenderer->ShadowOn();
	EnemyRenderer->SetDynamic();
	//PhysX(충돌)
	PhysXCapsule = CreateComponent<PhysXCapsuleComponent>();
	PhysXCapsule->SetPhysxMaterial(0, 0, 0);
	PhysXCapsule->CreatePhysXActors({ 90, 60, 90 });
	PhysXCapsule->GetDynamic()->setMass(80.0f);
	PhysXCapsule->SetWorldPosition({ 0, 100, 0 });
	BindPhysicsWithNet(PhysXCapsule);

	//공격 가능한 Enemy Collision
	//Monster 자체 콜리전
	MonsterCollision = CreateComponent<GameEngineCollision>(CollisionOrder::Enemy);
	MonsterCollision->GetTransform()->SetWorldScale(float4::ZERO);
	//Monster 공격 콜리전
	MonsterAttackCollision = CreateComponent<AttackCollision>(CollisionOrder::EnemyAttack);
	MonsterAttackCollision->GetTransform()->SetWorldScale(float4::ZERO);
	//Monster 공격 범위 인식 콜리전
	RN_MonsterCollision = CreateComponent<GameEngineCollision>(CollisionOrder::RN_Enemy);
	RN_MonsterCollision->GetTransform()->SetWorldScale(float4::ZERO);
	RN_MonsterCollision->SetColType(ColType::SPHERE3D);

	EnemyMeshLoad();
	EnemyTypeLoad();
	EnemyAnimationLoad();
	EnemyCreateFSM();
	EnemyCreateFSM_Client();

	//ObjectUpdatePacket를 수신하겠다는 것을 의미
	BindPacketFunction<ObjectUpdatePacket>(PacketEnum::ObjectUpdatePacket);

	//FsmChangePacket이 왔을때 어떻게 처리할 것인지
	BindPacketFunction<FsmChangePacket>(PacketEnum::FsmChangePacket, [this](std::shared_ptr<FsmChangePacket> _Packet)
	{
		SetFSMStateValue(_Packet->FsmState);
	});
}

void BaseEnemyActor::Update(float _DeltaTime)
{
	//if (MonsterCollision->GetTransform()->GetWorldScale() == float4::ZERO
	//	|| RN_MonsterCollision->GetTransform()->GetWorldScale() == float4::ZERO)
	//{
	//	MsgAssert("MonsterCollision의 크기를 설정해주지 않았습니다.");
	//}

	Update_NetworkTrans(_DeltaTime);
	RenderShake(_DeltaTime);
	MonsterSnatch(_DeltaTime);

	if (true == IsSnatch)
	{
		return;
	}

	if (false == NetworkManager::IsClient() && false == NetworkManager::IsServer())
	{
		RecognizeCollisionCheck(_DeltaTime);
		DamageCollisionCheck(_DeltaTime);
		EnemyFSM.Update(_DeltaTime);
	}
	else
	{
		if (NetControllType::ActiveControll == GetControllType())
		{
			RecognizeCollisionCheck(_DeltaTime);
			DamageCollisionCheck(_DeltaTime);
			EnemyFSM.Update(_DeltaTime);
		}
		else if (NetControllType::ActiveControll != GetControllType())
		{
			EnemyFSM_Client.Update(_DeltaTime);
		}
	}
}

void BaseEnemyActor::SuperArmorOn()
{
	IsSuperArmorValue = true;

	if (nullptr != SuperArmorOn_Callback) 
	{
		SuperArmorOn_Callback();
	}
}

void BaseEnemyActor::SuperArmorOff()
{
	IsSuperArmorValue = false;

	if (nullptr != SuperArmorOff_Callback)
	{
		SuperArmorOff_Callback();
	}
}

bool BaseEnemyActor::FloorCheck(float _Distance)
{
	float4 StartPosision = this->GetTransform()->GetWorldPosition();
	float4 Direction = float4::DOWN;
	float4 Results = float4::ZERO; // 레이가 닿은 결과값 궁금하면 이거 사용
	float CheckDistance = _Distance;

	bool IsResults = this->GetLevel()->RayCast(StartPosision, float4::DOWN, Results, _Distance);

	return IsResults;
}

float4 BaseEnemyActor::MonsterAndPlayerCross()
{
	std::vector<BasePlayerActor*>& Players = BasePlayerActor::GetPlayers();
	BasePlayerActor* Player = Players[0];

	if (nullptr == Player)
	{
		return float4::ZERO;
	}

	float4 EnemyPosition = this->GetTransform()->GetWorldPosition();
	float4 PlayerPosition = Player->GetTransform()->GetWorldPosition();
	float4 EnemyForWardVector = this->GetTransform()->GetWorldForwardVector();

	EnemyForWardVector.y = 0;
	EnemyForWardVector.Normalize();

	float4 ToPlayerVector = (PlayerPosition - EnemyPosition);
	ToPlayerVector.y = 0;
	ToPlayerVector.Normalize();

	float4 CrossResult = float4::Cross3DReturnNormal(EnemyForWardVector, ToPlayerVector);

	return CrossResult;
}

float BaseEnemyActor::MonsterAndPlayerDotProduct()
{
	std::vector<BasePlayerActor*>& Players = BasePlayerActor::GetPlayers();
	BasePlayerActor* Player = Players[0];

	if (nullptr == Player)
	{
		return 0.0f;
	}

	float4 EnemyPosition = this->GetTransform()->GetWorldPosition();
	float4 PlayerPosition = Player->GetTransform()->GetWorldPosition();
	float4 EnemyForWardVector = this->GetTransform()->GetWorldForwardVector();

	EnemyForWardVector.y = 0;
	EnemyForWardVector.Normalize();

	float4 ToPlayerVector = (PlayerPosition - EnemyPosition);
	ToPlayerVector.y = 0;
	ToPlayerVector.Normalize();

	float DotProductResult = float4::DotProduct3D(EnemyForWardVector, ToPlayerVector);

	return DotProductResult;
}

float BaseEnemyActor::MonsterAndPlayerDotProductDegree()
{
	std::vector<BasePlayerActor*>& Players = BasePlayerActor::GetPlayers();
	BasePlayerActor* Player = Players[0];

	float RotationValue = 0.0f;

	if (nullptr == Player)
	{
		return RotationValue;
	}

	float4 EnemyPosition = this->GetTransform()->GetWorldPosition();
	float4 PlayerPosition = Player->GetTransform()->GetWorldPosition();
	float4 EnemyForWardVector = this->GetTransform()->GetWorldForwardVector();

	EnemyForWardVector.y = 0;
	EnemyForWardVector.Normalize();

	float4 ToPlayerVector = (PlayerPosition - EnemyPosition);
	ToPlayerVector.y = 0;
	ToPlayerVector.Normalize();

	float4 CrossResult = MonsterAndPlayerCross();

	float4 Direct = PlayerPosition - EnemyPosition;
	float4 RotationDirectNormal = Direct.NormalizeReturn();
	RotationValue = float4::GetAngleVectorToVectorDeg(EnemyForWardVector, RotationDirectNormal);

	if (CrossResult.y < 0)
	{
		RotationValue = -RotationValue;
	}

	return RotationValue;
}

void BaseEnemyActor::RotationCheck()
{
	float4 CrossResult = MonsterAndPlayerCross();

	if (float4::ZERO == CrossResult)
	{
		return;
	}

	float DotProductResult = MonsterAndPlayerDotProductDegree();

	if (0.0f == DotProductResult)
	{
		return;
	}

	if (-20.0f <= DotProductResult && 20.0f >= DotProductResult)
	{
		EnemyRotationValue = EnemyRotation::Forward;
	}
	else if (CrossResult.y < 0)
	{
		if (-20.0f > DotProductResult && -165.0f <= DotProductResult)
		{
			EnemyRotationValue = EnemyRotation::Left;
		}
		else if (-165.0f > DotProductResult && -180.0f <= DotProductResult)
		{
			EnemyRotationValue = EnemyRotation::Left_180;
		}
	}
	else if (CrossResult.y > 0)
	{
		if (20.0f < DotProductResult && 165.0f >= DotProductResult)
		{
			EnemyRotationValue = EnemyRotation::Right;
		}
		else if (165.0f < DotProductResult && 180.0f >= DotProductResult)
		{
			EnemyRotationValue = EnemyRotation::Right_180;
		}
	}

	DotProductValue = DotProductResult;
}

void BaseEnemyActor::AttackDirectCheck()
{
	float4 CrossResult = MonsterAndPlayerCross();

	if (float4::ZERO == CrossResult)
	{
		return;
	}

	float DotProductResult = MonsterAndPlayerDotProductDegree();

	if (0.0f == DotProductResult)
	{
		return;
	}

	if (-50.0f <= DotProductResult && 50.0f >= DotProductResult)
	{
		EnemyHitDirValue = EnemyHitDirect::Forward;
	}
	else if (CrossResult.y < 0)
	{
		if (-50.0f > DotProductResult && -130.0f <= DotProductResult)
		{
			EnemyHitDirValue = EnemyHitDirect::Left;
		}
		else if (-130.0f > DotProductResult && -180.0f <= DotProductResult)
		{
			EnemyHitDirValue = EnemyHitDirect::Back;
		}
	}
	else if (CrossResult.y > 0)
	{
		if (50.0f < DotProductResult && 130.0f >= DotProductResult)
		{
			EnemyHitDirValue = EnemyHitDirect::Right;
		}
		else if (130.0f < DotProductResult && 180.0f >= DotProductResult)
		{
			EnemyHitDirValue = EnemyHitDirect::Back;
		}
	}
}

void BaseEnemyActor::SlerpCalculation()
{
	CurRotation = GetTransform()->GetWorldRotation();

	if (CurRotation.y <= 0.0f)
	{
		CurRotation.y += 360.f;
	}

	float4 Value = float4{ 0.0f, DotProductValue, 0.0f };

	GoalRotation = CurRotation + Value;

	if (GoalRotation.y <= 0.0f)
	{
		CurRotation.y += 360.f;
		GoalRotation = CurRotation + Value;
	}

	CurRotation.x = 0.0f;
	CurRotation.z = 0.0f;
	GoalRotation.x = 0.0f;
	GoalRotation.z = 0.0f;
}

void BaseEnemyActor::AnimationSlerpCalculation()
{
	StartFrame = EnemyRenderer->GetCurFrame();
	EndFrame = EnemyRenderer->GetEndFrame() -1;
	CalFrame = static_cast<float>(EndFrame - StartFrame);
	AnimationRotationTime = CalFrame * (1.f / 60.f);

	CurRotation = GetTransform()->GetWorldRotation();

	if (CurRotation.y <= 0.0f)
	{
		CurRotation.y += 360.f;
	}

	float4 Value = float4{ 0.0f, AnimationRotation, 0.0f };

	GoalRotation = CurRotation + Value;

	if (GoalRotation.y <= 0.0f)
	{
		CurRotation.y += 360.f;
		GoalRotation = CurRotation + Value;
	}

	CurRotation.x = 0.0f;
	CurRotation.z = 0.0f;
	GoalRotation.x = 0.0f;
	GoalRotation.z = 0.0f;
}

void BaseEnemyActor::AnimationSlerpTurn(float _DeltaTime)
{
	UINT Frmae = 0;

	if (StartFrame <= 0)
	{
		return;
	}

	if (nullptr != EnemyRenderer)
	{
		Frmae = EnemyRenderer->GetCurFrame();
	}

	if (StartFrame < Frmae)
	{
		return;
	}
	
	SlerpTime += _DeltaTime;
	RotationValue = float4::SLerpQuaternion(CurRotation, GoalRotation, SlerpTime);
}

void BaseEnemyActor::SlerpTurn(float _DeltaTime)
{
	SlerpTime += _DeltaTime;
	RotationValue = float4::SLerpQuaternion(CurRotation, GoalRotation, SlerpTime);
	PhysXCapsule->SetWorldRotation(RotationValue);
}

void BaseEnemyActor::AllDirectSetting()
{
	PhysXCapsule->AddWorldRotation({ 0, DotProductValue, 0 });
	
	ForwardDirect = GetTransform()->GetWorldForwardVector().NormalizeReturn();
	BackDirect = GetTransform()->GetWorldBackVector().NormalizeReturn();
	RightDirect = GetTransform()->GetWorldRightVector().NormalizeReturn();
	LeftDirect = GetTransform()->GetWorldLeftVector().NormalizeReturn();

	// 한프레임 차이나서 꺾어줘야함
	ForwardDirect.RotationYDeg(DotProductValue);
	BackDirect.RotationYDeg(DotProductValue);
	RightDirect.RotationYDeg(DotProductValue);
	LeftDirect.RotationYDeg(DotProductValue);
}

void BaseEnemyActor::PushDirectSetting()
{
	std::vector<BasePlayerActor*>& Players = BasePlayerActor::GetPlayers();
	BasePlayerActor* Player = Players[0];

	if (nullptr == Player)
	{
		return;
	}

	PushDirect = Player->GetTransform()->GetWorldForwardVector().NormalizeReturn();
}

void BaseEnemyActor::RenderShake(float _DeltaTime)
{
	if (false == IsRenderShaking)
	{
		ShakingCameraSetting = 1;
		return;
	}

	if (1 == ShakingCameraSetting)
	{
		RenderShakeTime = 0.0f;
		ShakingCount = 0;
		ShakingCameraSetting = 0;
		CurRenderPosition = EnemyRenderer->GetTransform()->GetLocalPosition();
		EnemyRenderer->GetTransform()->SetLocalPosition(CurRenderPosition);
	}

	RenderShakeTime += _DeltaTime;

	if (0.025f <= RenderShakeTime)
	{
		RenderShakeTime = 0.0f;

		if (ShakingMaxCount == ShakingCount)
		{
			ShakingCount = 0;
			IsRenderShaking = false;
			ShakingCameraSetting = 1;
			RenderShakeTime = 0.0f;
			EnemyRenderer->GetTransform()->SetLocalPosition(CurRenderPosition);
		}
		else if (0 == ShakingCount % 2)
		{
			++ShakingCount;
			EnemyRenderer->GetTransform()->SetLocalPosition({ CurRenderPosition.x + 3 , CurRenderPosition.y, CurRenderPosition.z - 3 });
		}
		else if (1 == ShakingCount % 2)
		{
			++ShakingCount;
			EnemyRenderer->GetTransform()->SetLocalPosition({ CurRenderPosition.x - 3 , CurRenderPosition.y, CurRenderPosition.z + 3 });
		}
	}
}

void BaseEnemyActor::SnatchCalculation()
{
	std::vector<BasePlayerActor*>& Players = BasePlayerActor::GetPlayers();
	BasePlayerActor* Player = Players[0];

	SnatchStartPosition = this->GetTransform()->GetWorldPosition();
	float4 Forword = Player->GetTransform()->GetWorldForwardVector() * 120.0f;
	SnatchEndPosition = Player->GetTransform()->GetWorldPosition() + Forword + float4{0.f, 20.f, 0.f};
}

void BaseEnemyActor::MonsterSnatch(float _DeltaTime)
{
	if (false == IsSnatch)
	{
		return;
	}

	SnatchTime += _DeltaTime * 3.0f;
	float4 LerpPosition = float4::LerpClamp(SnatchStartPosition, SnatchEndPosition, SnatchTime);
	//float4 LerpPosition = float4::SLerpQuaternion(SnatchStartPosition, SnatchEndPosition, SnatchTime);
	PhysXCapsule->SetWorldPosition(LerpPosition);

	if (1.0f <= SnatchTime)
	{
		SetMoveStop();
		IsSnatch = false;
		SnatchTime = 0.0f;
	}
}