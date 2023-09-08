#include "PrecompileHeader.h"
#include "BaseEnemyActor.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>
#include "EnemyActor_Normal.h"
#include "ContentsEnum.h"
#include "BasePlayerActor.h"

BaseEnemyActor::BaseEnemyActor()
{
}

BaseEnemyActor::~BaseEnemyActor()
{
}

void BaseEnemyActor::MonsterHit(const EnemyHitData& _HitData)
{
	GameEngineTransform* EnemyTransform = GetTransform();
	float4 HitPos = float4::ZERO;

	if (float4::ZERO == _HitData.AttackerPos)
	{
		HitPos = EnemyTransform->GetWorldPosition() + EnemyTransform->GetLocalForwardVector();
	}
	else
	{
		HitPos = _HitData.AttackerPos;
	}

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
		EnemyFSM.ChangeState(EnemyState::M_Hit);		
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
	CapsulCol = CreateComponent<PhysXCapsuleComponent>();
	CapsulCol->SetPhysxMaterial(0, 0, 0);
	CapsulCol->CreatePhysXActors({100, 70, 100});
	CapsulCol->SetWorldPosition({ 0, 100, 0 });

	//공격 가능한 Enemy Collision
	MonsterCollision=CreateComponent<GameEngineCollision>(CollisionOrder::Enemy);
	//주변 플레이어를 인식하는 Collision(1회용)
	RN_MonsterCollision = CreateComponent<GameEngineCollision>(CollisionOrder::RN_Enemy);
	//몬스터의 공격범위 Collision
	MonsterAttackRange = CreateComponent<GameEngineCollision>(CollisionOrder::RN_Enemy);
	
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
	RN_MonsterCollision->DebugOn();
	MonsterCollision->DebugOn();

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

void BaseEnemyActor::ChasePlayer()
{
	//Player를 인식했을때만
	if (true == RN_Player)
	{
		//서버에서 Player가 vector로 담아져있음. 그 벡터를 돌면서 따라감
		//거리값계산해서 가까운 플레이어로 추적하는기능 만들어야함
		for (BasePlayerActor* Players : BasePlayerActor::GetPlayers()) 
		{
			float4 EnemyPosition = EnemyRenderer->GetTransform()->GetWorldPosition();
			float4 PlayerPosition = Players->GetTransform()->GetWorldPosition();
			CapsulCol->SetMove((PlayerPosition - EnemyPosition));
		}
	}
}
