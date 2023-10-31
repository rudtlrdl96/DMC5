#include "PrecompileHeader.h"
#include "BaseEnemyActor.h"

#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>

#include "ContentsEnum.h"
#include "AttackCollision.h"
#include "BasePlayerActor.h"
#include "NetworkManager.h"
#include "FsmChangePacket.h"
#include "AttackCollision.h"
#include "Item_EnemyRedOrb.h"
#include "FXSystem.h"
#include "PlayerActor_Nero.h"


BaseEnemyActor::BaseEnemyActor()
{
}

BaseEnemyActor::~BaseEnemyActor()
{
}

void BaseEnemyActor::Start()
{
	if (false == GameEngineInput::IsKey("MonsterTest"))
	{
		GameEngineInput::CreateKey("MonsterTest", 'M');
	}

	//Render생성
	EnemyRenderer = CreateComponent<GameEngineFBXRenderer>();

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
	// 죽을때 드랍되는 레드오브
	RedOrbs = CreateComponent<Item_EnemyRedOrb>();
	MonsterEffectRenderer = CreateComponent<FXSystem>();	// 이펙트

	EnemyMeshLoad();

	EnemyRenderer->ShadowOn();
	EnemyRenderer->SetDynamic();

	EnemyTypeLoad();
	EnemyAnimationLoad();
	
	if (true == NetworkManager::IsClient())
	{
		EnemyCreateFSM_Client();
	}
	else if (true == NetworkManager::IsServer())
	{
		EnemyCreateFSM();
	}
	else
	{
		EnemyCreateFSM();
	}

	PlayerCheckInit();

	SetFsmPacketCallBack(std::bind(&BaseEnemyActor::SetFSMStateValue, this, std::placeholders::_1));
}

void BaseEnemyActor::Update(float _DeltaTime)
{
	AppearDelayTime += _DeltaTime;

	if (AppearDelayTime <= 1.0f)
	{
		return;
	}

	// 싱글, 서버, 클라이언트 상관없이 계속해서 체크되어야 하는 함수들
	{
		_DeltaTime *= GetTimeScale();
		EnemyRenderer->SetTimeScale(GetTimeScale());

		if (true == TimeStop)
		{
			TimeStop = false;
			if (true == IsTimeStop)
			{
				MonsterCollision->Off();
				MonsterAttackCollision->Off();
				PhysXCapsule->Off();
			}
			else if (false == IsTimeStop)
			{
				PhysXCapsule->On();
				MonsterCollision->On();
			}
		}

		RendererBurn(_DeltaTime);
		RenderShake(_DeltaTime);
		MonsterSnatch(_DeltaTime);
	}

	// 싱글 플레이일 때 실행
	if (false == NetworkManager::IsClient() && false == NetworkManager::IsServer())
	{
		if (true == GameEngineInput::IsDown("MonsterTest"))
		{
			EnemyHP = 1;
		}

		RecognizeCollisionCheck(_DeltaTime);
		DamageCollisionCheck(_DeltaTime);
		EnemyFSM.Update(_DeltaTime);
	}
	else 
	{
		// 서버 플레이일 때 실행
		if (NetControllType::ActiveControll == GetControllType())
		{
			if (true == GameEngineInput::IsDown("MonsterTest"))
			{
				EnemyHP = 1;
			}

			RecognizeCollisionCheck(_DeltaTime);
			DamageCollisionCheck(_DeltaTime);
			EnemyFSM.Update(_DeltaTime);
		}
		// 클라이언트 플레이일 때 실행
		else
		{
			if (EnemyCode::Qliphoth != EnemyCodeValue)
			{
				PhysXCapsule->SetLinearVelocityZero();
			}
			
			Update_NetworkTrans(_DeltaTime);
			DamageCollisionCheck_Client(_DeltaTime);
			EnemyFSM.Update(_DeltaTime);
			MonsterAttackCollision->Off();
		}
	}
}

/// <summary>
/// 액터 위치에서 아래 방향으로 Raycast를 실시하여 바닥(피직스 충돌체)을 체크한다.
/// </summary>
/// <param name="_Distance :">Raycasting 길이, 몬스터마다 다름</param>
bool BaseEnemyActor::FloorCheck(float _Distance)
{
	float4 StartPosision = this->GetTransform()->GetWorldPosition();
	float4 Direction = float4::DOWN;
	float4 Results = float4::ZERO; // 레이가 닿은 결과값 궁금하면 이거 사용
	float CheckDistance = _Distance;

	bool IsResults = this->GetLevel()->RayCast(StartPosision, float4::DOWN, Results, _Distance);

	return IsResults;
}

/// <summary>
/// 몬스터 최초 생성 시 Player 정보를 class BasePlayerActor* Player에 저장
/// </summary>
void BaseEnemyActor::PlayerCheckInit()
{
	std::vector<BasePlayerActor*>& Players = BasePlayerActor::GetPlayers();
	size_t Playersize = Players.size();

	if (0 == Players.size())
	{
		Player = nullptr;
		return;
	}

	Player = Players[0];

	if (true == NetworkManager::IsServer())
	{
		ServerPlayerID = Player->GetNetObjectID();
	}

	if (nullptr == Player)
	{
		MsgAssert("Player 세팅이 몬가 잘못됨");
	}
}

/// <summary>
/// RN_MonsterCollision에 충돌한 충돌체(Player) 정보를 class BasePlayerActor* Player에 저장
/// </summary>
/// <param name="_DeltaTime :">시간</param>
/// <param name="_Collision :">체크할 충돌체</param>
bool BaseEnemyActor::PlayerContactCheck(float _DeltaTime, GameEngineCollision* _Collision)
{
	//ContactDelayTime += _DeltaTime;

	//if (10.0f >= ContactDelayTime)
	//{
	//	return false;
	//}

	ContactDelayTime = 0.0f;

	GameEngineCollision* CheckCollision = _Collision;
	BasePlayerActor* ContactPlayer = dynamic_cast<BasePlayerActor*>(CheckCollision->GetActor());

	if (nullptr == ContactPlayer)
	{
		return false;
	}

	bool IsContact = false;

	if (false == NetworkManager::IsClient() && false == NetworkManager::IsServer())
	{
		//Player = ContactPlayer;
		IsContact = true;
	}
	else
	{
		int ContactID = ContactPlayer->GetNetObjectID();
		int PlayerID = Player->GetNetObjectID();

		if (ContactID == PlayerID)
		{
			Player = ContactPlayer;
			IsContact = true;
		}
		else
		{
			IsContact = false;
		}
	}

	return IsContact;
}

/// <summary>
/// RN_MonsterCollision에 충돌한 충돌체(Player) 정보를 class BasePlayerActor* Player에 저장
/// </summary>
/// <param name="_DeltaTime :">시간</param>
/// <param name="_Collision :">체크할 충돌체</param>
bool BaseEnemyActor::PlayerContactCheck_Normal(GameEngineCollision* _Collision)
{
	GameEngineCollision* CheckCollision = _Collision;
	BasePlayerActor* ContactPlayer = dynamic_cast<BasePlayerActor*>(CheckCollision->GetActor());

	if (nullptr == ContactPlayer)
	{
		return false;
	}

	bool IsContact = false;

	if (false == NetworkManager::IsClient() && false == NetworkManager::IsServer())
	{
		//Player = ContactPlayer;
		IsContact = true;
	}
	else
	{
		int ContactID = ContactPlayer->GetNetObjectID();
		int PlayerID = Player->GetNetObjectID();

		if (ContactID == PlayerID)
		{
			IsContact = true;
		}
	}

	return IsContact;
}

/// <summary>
/// 자신을 공격한 충돌체(Player) 정보를 class BasePlayerActor* Player에 저장
/// </summary>
/// <param name="_Collision :">체크할 충돌체</param>
void BaseEnemyActor::PlayerAttackCheck(GameEngineCollision* _Collision)
{
	GameEngineCollision* CheckCollision = _Collision;
	BasePlayerActor* ContactPlayer = dynamic_cast<BasePlayerActor*>(CheckCollision->GetActor());

	if (nullptr == ContactPlayer)
	{
		return;
	}

	if (false == NetworkManager::IsClient() && false == NetworkManager::IsServer())
	{
		Player = ContactPlayer;
	}
	else
	{
		int ContactID = ContactPlayer->GetNetObjectID();
		int PlayerID = Player->GetNetObjectID();

		if (ContactID == PlayerID)
		{
			Player = ContactPlayer;
		}
		else
		{
			std::vector<BasePlayerActor*>& Players = BasePlayerActor::GetPlayers();
			size_t Playersize = Players.size();

			for (size_t i = 0; i < Playersize; i++)
			{
				int PlayersID = Players[i]->GetNetObjectID();

				if (ContactID == PlayersID)
				{
					Player = Players[i];
				}
			}
		}
	}
}

/// <summary>
/// Player에 대해 현재 몬스터의 위치를 외적합니다. (어디로 돌 것인지 결정하기 위함)
/// </summary>
float4 BaseEnemyActor::MonsterAndPlayerCross()
{
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

/// <summary>
/// Player에 대해 현재 몬스터의 위치를 내적합니다. (해당 방향으로 몇도를 회전해야 하는지 알기 위함)
/// </summary>
float BaseEnemyActor::MonsterAndPlayerDotProduct()
{
	if (nullptr == Player)
	{
		return 0.0f;
	}

	float4 EnemyPosition = this->GetTransform()->GetWorldPosition();
	float4 PlayerPosition = Player->GetTransform()->GetWorldPosition();
	float4 EnemyForWardVector = this->GetTransform()->GetWorldForwardVector();

	PlayerPosition.y = 0.0f;
	EnemyPosition.y = 0.0f;

	EnemyForWardVector.y = 0;
	EnemyForWardVector.Normalize();

	float4 ToPlayerVector = (PlayerPosition - EnemyPosition);
	ToPlayerVector.y = 0;
	ToPlayerVector.Normalize();

	float DotProductResult = float4::DotProduct3D(EnemyForWardVector, ToPlayerVector);

	if (true == isnan(DotProductResult))
	{
		DotProductResult = 0.0f;
		return DotProductResult;
	}

	return DotProductResult;
}

/// <summary>
/// Player에 대해 현재 몬스터의 위치를 내적합니다. Degree로 값을 반환해줍니다.
/// </summary>
float BaseEnemyActor::MonsterAndPlayerDotProductDegree()
{
	float RotationValue = 0.0f;

	if (nullptr == Player)
	{
		return RotationValue;
	}

	float4 EnemyPosition = this->GetTransform()->GetWorldPosition();
	float4 PlayerPosition = Player->GetTransform()->GetWorldPosition();
	float4 EnemyForWardVector = this->GetTransform()->GetWorldForwardVector();

	PlayerPosition.y = 0.0f;
	EnemyPosition.y = 0.0f;

	EnemyForWardVector.y = 0;
	EnemyForWardVector.Normalize();

	float4 ToPlayerVector = (PlayerPosition - EnemyPosition);
	ToPlayerVector.y = 0;
	ToPlayerVector.Normalize();

	float4 CrossResult = MonsterAndPlayerCross();

	RotationValue = float4::GetAngleVectorToVectorDeg(EnemyForWardVector, ToPlayerVector);

	if (true == isnan(RotationValue))
	{
		RotationValue = 0.0f;
		return RotationValue;
	}

	if (CrossResult.y < 0)
	{
		RotationValue = -RotationValue;
	}

	return RotationValue;
}

/// <summary>
/// Player에 대해 현재 몬스터의 위치를 외적, 내적을 한 뒤 어떤 방향으로 회전해야하는지 EnemyRotation EnemyRotationValue에 값을 저장합니다.
/// </summary>
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
		EnemyRotationValue = EnemyRotation::Forward;
		DotProductValue = 0.0f;
		return;
	}

	if (-20.0f <= DotProductResult && 20.0f >= DotProductResult)
	{
		EnemyRotationValue = EnemyRotation::Forward;
	}
	else if (CrossResult.y < 0)
	{
		if (-20.0f > DotProductResult && -75.0f <= DotProductResult)
		{
			EnemyRotationValue = EnemyRotation::Left;
		}
		else if (-75.0f > DotProductResult && -165.0f <= DotProductResult)
		{
			EnemyRotationValue = EnemyRotation::Left_90;
		}
		else if (-165.0f > DotProductResult && -180.0f <= DotProductResult)
		{
			EnemyRotationValue = EnemyRotation::Left_180;
		}
	}
	else if (CrossResult.y > 0)
	{
		if (20.0f < DotProductResult && 75.0f >= DotProductResult)
		{
			EnemyRotationValue = EnemyRotation::Right;
		}
		else if (75.0f < DotProductResult && 165.0f >= DotProductResult)
		{
			EnemyRotationValue = EnemyRotation::Right_90;
		}
		else if (165.0f < DotProductResult && 180.0f >= DotProductResult)
		{
			EnemyRotationValue = EnemyRotation::Right_180;
		}
	}

	DotProductValue = DotProductResult;
}

/// <summary>
/// Player에 대해 현재 몬스터의 위치를 외적, 내적을 한 뒤 어떤 방향에서 공격 받았는지를 EnemyHitDirect EnemyHitDirValue에 저장합니다.
/// </summary>
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

/// <summary>
/// Slerp로 회전해야 하는 경우에 대한 값을 계산합니다. 계산 결과는 float4 CurRotation, float4 GoalRotation에 저장됩니다. Y축에 대한 회전만 따집니다.
/// </summary>
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
	//SlerpTime = 0.0f;
}

/// <summary>
/// 일정 시간 동안 SlerpCalculation()()로 계산된  값을 향해 몬스터를 Slerp시킨다.
/// </summary>
/// <param name="_DeltaTime :">시간</param>
void BaseEnemyActor::SlerpTurn(float _DeltaTime)
{
	SlerpTime += _DeltaTime;
	RotationValue = float4::SLerpQuaternion(CurRotation, GoalRotation, SlerpTime);
	PhysXCapsule->SetWorldRotation(RotationValue);
}

/// <summary>
/// 몬스터의 현재 위치와 각도에 대해 전방, 후방, 좌측, 우측에 대한 노말 벡터값을 결정해준다. Phsysics와 Engine 연산이 1프레임 차이나기 때문에 벡터를 임의로 회전시킨다.
/// </summary>
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

/// <summary>
/// 몬스터의 현재 위치와 각도에 대해 전방, 후방, 좌측, 우측에 대한 노말 벡터값을 결정해준다.
/// </summary>
void BaseEnemyActor::AllDirectSetting_Normal()
{
	ForwardDirect = GetTransform()->GetWorldForwardVector().NormalizeReturn();
	BackDirect = GetTransform()->GetWorldBackVector().NormalizeReturn();
	RightDirect = GetTransform()->GetWorldRightVector().NormalizeReturn();
	LeftDirect = GetTransform()->GetWorldLeftVector().NormalizeReturn();
}

/// <summary>
/// 몬스터의 현재 위치와 각도에 대해 날아갈 방향을 PushDirect에 노말벡터로 저장한다.
/// </summary>
void BaseEnemyActor::PushDirectSetting()
{
	if (nullptr == Player)
	{
		return;
	}

	PushDirect = Player->GetTransform()->GetWorldForwardVector().NormalizeReturn();
}

/// <summary>
/// 슈퍼아머 상태에서 히트 시 랜더러를 좌우로 흔든다.
/// </summary>
/// <param name="_DeltaTime :">0.025초마다 1회 떨림 체크를 위한 시간</param>
void BaseEnemyActor::RenderShake(float _DeltaTime)
{
	if (true == DeathValue)
	{
		return;
	}

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

/// <summary>
/// 버스터 히트 시 몬스터를 Player FBX Bone의 어떤 위치에 부착할지 결정한다.
/// </summary>
/// <param name="_attachposition :">FBX Bone에 부착된 상태에서의 랜더러 Position</param>
void BaseEnemyActor::BusterCalculation(float4 _attachposition)
{
	if (nullptr == Player)
	{
		return;
	}

	float4 Forword = Player->GetTransform()->GetWorldForwardVector() * 120.0f;
	float4 BusterPosition = Player->GetTransform()->GetWorldPosition() + Forword + float4{0.f, 40.f, 0.f};
	
	PhysXCapsule->SetWorldPosition(BusterPosition);
	Player->GetPlayerRenderer()->SetAttachTransform("R_Hand", EnemyRenderer->GetTransform(), _attachposition/*, float4(0.0f, 0.0f, 0.0f)*/);
}

void BaseEnemyActor::BusterCalculation_Client(float4 _attachposition)
{
	if (nullptr == Player)
	{
		return;
	}

	if (BusterClientStart == true)
	{
		Player->GetPlayerRenderer()->SetAttachTransform("R_Hand", EnemyRenderer->GetTransform(), _attachposition/*, float4(0.0f, 0.0f, 0.0f)*/);
	}
	else
	{
		std::vector<BasePlayerActor*>& Players = BasePlayerActor::GetPlayers();
		size_t Playersize = Players.size();

		for (size_t i = 0; i < Playersize; i++)
		{
			int PlayersID = Players[i]->GetNetObjectID();

			if (PlayersID == ServerPlayerID)
			{
				ServerPlayer = Players[i];
				ServerPlayer->GetPlayerRenderer()->SetAttachTransform("R_Hand", EnemyRenderer->GetTransform(), _attachposition/*, float4(0.0f, 0.0f, 0.0f)*/);
				break;
			}
		}
	}
}

/// <summary>
/// 버스터 종료 시 몬스터 랜더러의 위치를 원래 위치로 되돌린다.
/// </summary>
void BaseEnemyActor::BusterEnd()
{
	if (nullptr == Player)
	{
		return;
	}

	std::vector<BasePlayerActor*>& Players = BasePlayerActor::GetPlayers();
	size_t Playersize = Players.size();

	for (size_t i = 0; i < Playersize; i++)
	{
		PlayerActor_Nero* NeP = dynamic_cast<PlayerActor_Nero*>(Players[i]);

		if (nullptr != NeP)
		{
			Players[i]->GetPlayerRenderer()->SetDettachTransform();
		}
	}

	if (EnemyCode::Empusa == EnemyCodeValue)
	{
		EnemyRenderer->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
		EnemyRenderer->GetTransform()->AddLocalPosition({ 0.0f, -50.0f, 0.0f });
	}
	else
	{
		EnemyRenderer->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
		EnemyRenderer->GetTransform()->AddLocalPosition({ 0.0f, -45.0f, 0.0f });
	}
}

void BaseEnemyActor::BusterEnd_Client()
{
	if (nullptr == Player)
	{
		return;
	}

	if (BusterClientStart == true)
	{
		BusterClientStart = false;
		//Player->GetPlayerRenderer()->SetDettachTransform();
	}
	else
	{
		//ServerPlayer->GetPlayerRenderer()->SetDettachTransform();
	}

	std::vector<BasePlayerActor*>& Players = BasePlayerActor::GetPlayers();
	size_t Playersize = Players.size();

	for (size_t i = 0; i < Playersize; i++)
	{
		PlayerActor_Nero* NeP = dynamic_cast<PlayerActor_Nero*>(Players[i]);

		if (nullptr != NeP)
		{
			Players[i]->GetPlayerRenderer()->SetDettachTransform();
		}
	}

	if (EnemyCode::Empusa == EnemyCodeValue)
	{
		EnemyRenderer->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
		EnemyRenderer->GetTransform()->AddLocalPosition({ 0.0f, -50.0f, 0.0f });
	}
	else
	{
		EnemyRenderer->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
		EnemyRenderer->GetTransform()->AddLocalPosition({ 0.0f, -45.0f, 0.0f });
	}
}

/// <summary>
/// 스내치 히트 시 Lerp할 몬스터와 Player의 위치값을 float4 SnatchStartPosition, float4 SnatchEndPosition에 값을 저장한다.
/// </summary>
void BaseEnemyActor::SnatchCalculation()
{
	if (nullptr == Player)
	{
		return;
	}

	SnatchStartPosition = this->GetTransform()->GetWorldPosition();
	float4 Forword = Player->GetTransform()->GetWorldForwardVector() * 120.0f;
	SnatchEndPosition = Player->GetTransform()->GetWorldPosition() + Forword + float4{0.f, -20.f, 0.f};
}

/// <summary>
/// 일정 시간(1.0f) 동안 계산된 SnatchCalculation()로 계산된 값을 향해 몬스터를 Lerp시킨다.
/// </summary>
/// <param name="_DeltaTime :">1.0초를 체크할 시간</param>
void BaseEnemyActor::MonsterSnatch(float _DeltaTime)
{
	if (true == DeathValue)
	{
		return;
	}

	if (false == IsSnatch)
	{
		return;
	}

	SnatchTime += _DeltaTime * 3.0f;
	float4 LerpPosition = float4::LerpClamp(SnatchStartPosition, SnatchEndPosition, SnatchTime);
	PhysXCapsule->SetWorldPosition(LerpPosition);

	if (1.0f <= SnatchTime)
	{
		SetMoveStop();
		IsSnatch = false;
		SnatchTime = 0.0f;
	}
}

/// <summary>
/// 입력된 DamageType에 따라 일정시간동안 타임스케일을 낮춥니다
/// </summary>
/// <param name="_Type :">DamageType</param>
void BaseEnemyActor::HitStop(DamageType _Type)
{
	float _TimeScale = 0.1f;
	float StopTime = 0.0f;

	switch (_Type)
	{
	case DamageType::VergilLight:
		StopTime = 0.01f;
		break;
	case DamageType::Light:
		StopTime = 0.05f;
		break;
	case DamageType::Heavy:
		StopTime = 0.08f;
		break;
	case DamageType::Air:
		StopTime = 0.05f;
		break;
	case DamageType::Slam:
		StopTime = 0.08f;
		break;
	case DamageType::Stun:
		return;
	default:
		break;
	}

	SetTimeScale(_TimeScale);

	GetLevel()->TimeEvent.AddEvent(StopTime, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
	{
		SetTimeScale(1.0f);
	});
}

/// <summary>
/// 일정시간동안 타임스케일을 0으로 합니다. _DeltaTime 초 후 타임스케일은 1이 됩니다. CapsuleCollision은 Off됩니다
/// </summary>
/// <param name="_DeltaTime :">타임 스케일이 1이 되기까지의 시간을 체크하기 위함</param>
void BaseEnemyActor::StopTime(float _DeltaTime)
{
	TimeStop = true;
	IsTimeStop = true;
	SetTimeScale(0.0f);
	GetLevel()->TimeEvent.AddEvent(_DeltaTime, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
	{
		TimeStop = true;
		IsTimeStop = false;
		SetTimeScale(1.0f);
	});
}

void BaseEnemyActor::RendererBurn(float _DeltaTime)
{
	if (false == IsBurn)
	{
		return;
	}

	DeathColor += _DeltaTime;

	if (DeathColor <= 1.1f)
	{
		EnemyRenderer->SetBaseColor({ 0.0f, 0.0f, DeathColor });

		if (nullptr != LeftWeapon)
		{
			LeftWeapon->SetBaseColor({ 0.0f, 0.0f, DeathColor });
		}
		if (nullptr != RightWeapon)
		{
			RightWeapon->SetBaseColor({ 0.0f, 0.0f, DeathColor });
		}
	}

	if (DeathColor >= 1.0f)
	{
		// 싱글 플레이일 때 실행
		if (false == NetworkManager::IsClient() && false == NetworkManager::IsServer())
		{
			Death();
		}
		else
		{
			if (NetControllType::ActiveControll == GetControllType())
			{
				Death();
			}
		}
	}
}

void BaseEnemyActor::DeathCheck()
{
	if (EnemyHP <= 0)
	{
		DeathValue = true;
		RedOrbDrop();
		DeathEvent();
		BusterEnd();
	}

	if (true == DeathValue && false == DeathSettig)
	{
		DeathSettig = true;
		MonsterCollision->Off();
		RN_MonsterCollision->Off();
		PhysXCapsule->Off();
	}
}

void BaseEnemyActor::DeathSetting_Client()
{
	RedOrbDrop();
	MonsterCollision->Off();
	RN_MonsterCollision->Off();
	PhysXCapsule->Off();
}

void BaseEnemyActor::PlayDamageEvent(DamageType _Type, DamageSoundType _SoundType)
{
	switch (_Type)
	{
	case DamageType::VergilLight:
	case DamageType::Light:
		MonsterEffectRenderer->PlayFX("Enemy_Damage_Light.effect");
		break;
	case DamageType::Heavy:
	case DamageType::Air:
	case DamageType::Slam:
		MonsterEffectRenderer->PlayFX("Enemy_Damage_Heavy.effect");
		break;
	default:
		break;
	}

	switch (_SoundType)
	{
	case DamageSoundType::None:
		break;
	case DamageSoundType::Sword:
		Sound.PlayRandom("Enemy_Damage_", 0, 3);
		break;
	case DamageSoundType::Magic:
		Sound.PlayRandom("Enemy_Damage_", 4, 5);
		break;
	case DamageSoundType::Katana:
		Sound.PlayRandom("Enemy_Damage_", 0, 3);
		break;
	case DamageSoundType::Blunt:
		Sound.PlayRandom("Enemy_Damage_", 7, 8);
		break;
	case DamageSoundType::Gun:
		Sound.Play("Enemy_Damage_", 11);
		break;
	default:
		break;
	}
}

void BaseEnemyActor::DeathEvent()
{
	for (const std::function<void()>& CallBack : DeathCallbacks)
	{
		CallBack();
	}

	DeathCallbacks.clear();
}

void BaseEnemyActor::RedOrbDrop()
{
	RedOrbs->OrbOn();
}
