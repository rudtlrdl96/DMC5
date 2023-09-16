#include "PrecompileHeader.h"
#include "BaseEnemyActor.h"

#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>

#include "ContentsEnum.h"
#include "AttackCollision.h"
#include "BasePlayerActor.h"

BaseEnemyActor::BaseEnemyActor()
{
}

BaseEnemyActor::~BaseEnemyActor()
{
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

void BaseEnemyActor::MonsterHit(const EnemyHitData& _HitData)
{
	float4 HitPos = _HitData.AttackerPos;
	EnemyHP -= _HitData.Damage;
	HitDir = GetHitDir(HitPos);

	switch (_HitData.Type)
	{
	case MonsterDamageType::None:
		MsgAssert("BaseEnemyActor MonsterHit(const EnemyHitData& _HitData) : 설정되지 않은 피격 타입 입니다");
		break;

	case MonsterDamageType::AngleDamage:
	{
		static bool MsgCheck = false;
		if (false == MsgCheck)
		{
			MsgTextBox("BaseEnemyActor MonsterHit(const EnemyHitData& _HitData) : [AngleDamage] 아직 구현하지 않은 타입의 데미지입니다.");
		}
		break;
	}
	case MonsterDamageType::Air:
	{
		//EnemyFSM.ChangeState(EnemyState::M_Hit);		
		break;
	}
	case MonsterDamageType::Snatch:
	{
		static bool MsgCheck = false;
		if (false == MsgCheck)
		{
			MsgTextBox("BaseEnemyActor MonsterHit(const EnemyHitData& _HitData) : [Snatch] 아직 구현하지 않은 타입의 데미지입니다.");
		}
		break;
	}
	case MonsterDamageType::Blown:
	{
		static bool MsgCheck = false;
		if (false == MsgCheck)
		{
			MsgTextBox("BaseEnemyActor MonsterHit(const EnemyHitData& _HitData) : [Blown] 아직 구현하지 않은 타입의 데미지입니다.");
		}
		break;
	}
	case MonsterDamageType::Drilling:
	{
		static bool MsgCheck = false;
		if (false == MsgCheck)
		{
			MsgTextBox("BaseEnemyActor MonsterHit(const EnemyHitData& _HitData) : [Drilling] 아직 구현하지 않은 타입의 데미지입니다.");
		}
		break;
	}
	case MonsterDamageType::Launching:
	{
		static bool MsgCheck = false;
		if (false == MsgCheck)
		{
			MsgTextBox("BaseEnemyActor MonsterHit(const EnemyHitData& _HitData) : [Launching] 아직 구현하지 않은 타입의 데미지입니다.");
		}
		break;
	}
	case MonsterDamageType::Slam:
	{
		static bool MsgCheck = false;
		if (false == MsgCheck)
		{
			MsgTextBox("BaseEnemyActor MonsterHit(const EnemyHitData& _HitData) : [Slam] 아직 구현하지 않은 타입의 데미지입니다.");
		}
		break;
	}
	case MonsterDamageType::Downward:
	{
		static bool MsgCheck = false;
		if (false == MsgCheck)
		{
			MsgTextBox("BaseEnemyActor MonsterHit(const EnemyHitData& _HitData) : [Downward] 아직 구현하지 않은 타입의 데미지입니다.");
		}
		break;
	}
	case MonsterDamageType::Stun:
	{
		static bool MsgCheck = false;
		if (false == MsgCheck)
		{
			MsgTextBox("BaseEnemyActor MonsterHit(const EnemyHitData& _HitData) : [Stun] 아직 구현하지 않은 타입의 데미지입니다.");
		}
		break;
	}
	case MonsterDamageType::Hold:
	{
		static bool MsgCheck = false;
		if (false == MsgCheck)
		{
			MsgTextBox("BaseEnemyActor MonsterHit(const EnemyHitData& _HitData) : [Hold] 아직 구현하지 않은 타입의 데미지입니다.");
		}
		break;
	}
	default:
		MsgAssert("BaseEnemyActor MonsterHit(const EnemyHitData& _HitData) : 잘못된 피격 타입이 들어왔습니다.");
		break;
	}
}

EnemyHitDir BaseEnemyActor::GetHitDir(const float4& _WorldPos)
{
	EnemyHitDir ResultDir = EnemyHitDir::None;

	GameEngineTransform* EnemyTrans = GetTransform();

	float DotCos = EnemyTrans->GetRotation_Forward(_WorldPos);

	if (315.0f >  DotCos && 225.0f < DotCos)
	{
		ResultDir = EnemyHitDir::Left;			
	}
	else if(225.0f >= DotCos && 135.0f < DotCos)
	{
		ResultDir = EnemyHitDir::Back;
	}
	else if (135.0f >= DotCos && 45.0f < DotCos)
	{
		ResultDir = EnemyHitDir::Right;
	}
	else
	{
		ResultDir = EnemyHitDir::Forward;
	}

#ifdef _DEBUG

	switch (ResultDir)
	{
	case EnemyHitDir::Forward:
		BaseLog::PushLog(LogOrder::Enemy, "Hit Forward [" + std::to_string(GetID()) + "]");
		break;
	case EnemyHitDir::Back:
		BaseLog::PushLog(LogOrder::Enemy, "Hit Back [" + std::to_string(GetID()) + "]");
		break;
	case EnemyHitDir::Left:
		BaseLog::PushLog(LogOrder::Enemy, "Hit Left [" + std::to_string(GetID()) + "]");
		break;
	case EnemyHitDir::Right:
		BaseLog::PushLog(LogOrder::Enemy, "Hit Right [" + std::to_string(GetID()) + "]");
		break;
	default:
		break;
	}
#endif // _DEBUG

	return ResultDir;
}

void BaseEnemyActor::Start()
{
	//Render생성
	EnemyRenderer = CreateComponent<GameEngineFBXRenderer>();
	//PhysX(충돌)
	PhysXCapsule = CreateComponent<PhysXCapsuleComponent>();
	PhysXCapsule->SetPhysxMaterial(0, 0, 0);
	PhysXCapsule->CreatePhysXActors({100, 70, 100});
	PhysXCapsule->SetWorldPosition({ 0, 100, 0 });

	//공격 가능한 Enemy Collision
	MonsterCollision = CreateComponent<GameEngineCollision>(CollisionOrder::Enemy);
	MonsterCollision->GetTransform()->SetWorldScale(float4::ZERO);
	//주변 플레이어를 인식하는 Collision(1회용)
	RN_MonsterCollision = CreateComponent<GameEngineCollision>(CollisionOrder::RN_Enemy);
	RN_MonsterCollision->GetTransform()->SetWorldScale(float4::ZERO);
	//몬스터의 공격범위 Collision
	MonsterAttackRange = CreateComponent<GameEngineCollision>(CollisionOrder::RN_Enemy);
	MonsterAttackRange->GetTransform()->SetWorldScale(float4::ZERO);
	//ForWardCollision
	ForWardCollision = CreateComponent<GameEngineCollision>(CollisionOrder::RN_Enemy);
	ForWardCollision->GetTransform()->SetWorldScale(float4::ZERO);
	
	//초기화
	EnemyCodeValue = EnemyCode::None;
	EnemyTypeValue = EnemyType::None;
	EnemySizeValue = EnemySize::None;

	// Debug Key
	if (false == GameEngineInput::IsKey("EnemyDebug_Ctrl"))
	{
		GameEngineInput::CreateKey("EnemyDebug_Ctrl", VK_LCONTROL);
		GameEngineInput::CreateKey("DebugSwitch", '0');
		GameEngineInput::CreateKey("EnemyDebug_Forward", VK_F9);
		GameEngineInput::CreateKey("EnemyDebug_Left", VK_F10);
		GameEngineInput::CreateKey("EnemyDebug_Back", '2');
		GameEngineInput::CreateKey("EnemyDebug_Right", '1');
		GameEngineInput::CreateKey("EnemyDebug_Attack", VK_F11);
		GameEngineInput::CreateKey("EnemyDebug_6", '6');
		GameEngineInput::CreateKey("EnemyDebug_7", '7');
		GameEngineInput::CreateKey("EnemyDebug_8", '8');
		GameEngineInput::CreateKey("EnemyDebug_9", '9');
	}
	EnemyMeshLoad();
	EnemyTypeLoad();
	EnemyAnimationLoad();
	EnemyCreateFSM();
}


void BaseEnemyActor::Update(float _DeltaTime)
{
	NetControllType Type = GetControllType();
	switch (Type)
	{
	case NetControllType::UserControll:
		UserUpdate(_DeltaTime);
		break;
	case NetControllType::NetControll:
		ServerUpdate(_DeltaTime);
		break;
	default:
		break;
	}

	if (MonsterCollision->GetTransform()->GetWorldScale() == float4::ZERO)
	{
		MsgAssert("MonsterCollision의 크기를 설정해주지 않았습니다.");
	}
	if (RN_MonsterCollision->GetTransform()->GetWorldScale() == float4::ZERO)
	{
		MsgAssert("RN_MonsterCollision의 크기를 설정해주지 않았습니다.");
	}
	if (MonsterAttackRange->GetTransform()->GetWorldScale() == float4::ZERO)
	{
		MsgAssert("MonsterAttackRange의 크기를 설정해주지 않았습니다.");
	}
	if (ForWardCollision->GetTransform()->GetWorldScale() == float4::ZERO)
	{
		MsgAssert("ForWardCollision의 크기를 설정해주지 않았습니다.");
	}
}

void BaseEnemyActor::UserUpdate(float _DeltaTime)
{
	if (true == EnemyFSM.IsValid())
	{
		EnemyFSM.Update(_DeltaTime);
	}
}

void BaseEnemyActor::ServerUpdate(float _DeltaTime)
{
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

void BaseEnemyActor::ChasePlayer(float _DeltaTime)
{
	//서버에서 Player가 vector로 담아져있음. 그 벡터를 돌면서 따라감
	for (BasePlayerActor* Players : BasePlayerActor::GetPlayers())
	{
		float4 EnemyPosition = EnemyRenderer->GetTransform()->GetWorldPosition();
		float4 PlayerPosition = Players->GetTransform()->GetWorldPosition();
		PhysXCapsule->SetMove((PlayerPosition - EnemyPosition));
		//자연스럽게 돌리기
		ColValue = ForWardCollision->Collision(CollisionOrder::RN_Player, ColType::OBBBOX3D, ColType::OBBBOX3D);
		if (nullptr == ColValue)
		{
			float4 EnemyForWardVector = EnemyRenderer->GetTransform()->GetWorldForwardVector();
			EnemyForWardVector.y = 0;
			EnemyForWardVector.Normalize();

			float4 ToPlayerVector = (PlayerPosition - EnemyPosition);
			ToPlayerVector.y = 0;
			ToPlayerVector.Normalize();

			float4 CrossVector = float4::Cross3DReturnNormal(EnemyForWardVector, ToPlayerVector);
			if (CrossVector.y < 0){	RotateValue = -2;}
			else{RotateValue = 2;}

			EnemyRenderer->GetTransform()->AddLocalRotation({ 0,RotateValue,0 });
			ForWardCollision->GetTransform()->AddLocalRotation({ 0,RotateValue,0 });
		}
	}
}

float4 BaseEnemyActor::CrossMonsterAndPlayer()
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

float BaseEnemyActor::DotProductMonsterAndPlayer()
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

void BaseEnemyActor::CheckHeadingRotationValue()
{
	float4 CrossResult = CrossMonsterAndPlayer();

	if (float4::ZERO == CrossResult)
	{
		return;
	}

	float DotProductResult = RotationToPlayerValue();

	if (0.0f == DotProductResult)
	{
		return;
	}

	if (0.0f <= DotProductResult && 15.0f >= DotProductResult)
	{
		EnemyRotationValue = EnemyRotation::Forward;
	}
	else if (CrossResult.y < 0)
	{
		if (-50.0f > DotProductResult && -140.0f <= DotProductResult)
		{
			EnemyRotationValue = EnemyRotation::Left_90;
		}
		else if (-140.0f > DotProductResult && -180.0f <= DotProductResult)
		{
			EnemyRotationValue = EnemyRotation::Left_180;
		}
		else
		{
			EnemyRotationValue = EnemyRotation::Left;
		}
	}
	else if (CrossResult.y > 0)
	{
		if (50.0f < DotProductResult && 140.0f >= DotProductResult)
		{
			EnemyRotationValue = EnemyRotation::Right_90;
		}
		else if (140.0f < DotProductResult && 180.0f >= DotProductResult)
		{
			EnemyRotationValue = EnemyRotation::Right_180;
		}
		else
		{
			EnemyRotationValue = EnemyRotation::Right;
		}
	}
	else
	{
		MsgAssert("회전의 내적과 외적이 몬가 잘못됨");
	}
}

float BaseEnemyActor::RotationToPlayerValue()
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

	float4 CrossResult = CrossMonsterAndPlayer();

	float4 Direct = PlayerPosition - EnemyPosition;
	float4 RotationDirectNormal = Direct.NormalizeReturn();
	RotationValue = float4::GetAngleVectorToVectorDeg(EnemyForWardVector, RotationDirectNormal);

	if (CrossResult.y < 0)
	{
		RotationValue = -RotationValue;
	}

	return RotationValue;
}