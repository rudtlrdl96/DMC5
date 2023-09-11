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
		MsgAssert("BaseEnemyActor MonsterHit(const EnemyHitData& _HitData) : �������� ���� �ǰ� Ÿ�� �Դϴ�");
		break;

	case MonsterDamageType::AngleDamage:
	{
		static bool MsgCheck = false;
		if (false == MsgCheck)
		{
			MsgTextBox("BaseEnemyActor MonsterHit(const EnemyHitData& _HitData) : [AngleDamage] ���� �������� ���� Ÿ���� �������Դϴ�.");
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
			MsgTextBox("BaseEnemyActor MonsterHit(const EnemyHitData& _HitData) : [Snatch] ���� �������� ���� Ÿ���� �������Դϴ�.");
		}
		break;
	}
	case MonsterDamageType::Blown:
	{
		static bool MsgCheck = false;
		if (false == MsgCheck)
		{
			MsgTextBox("BaseEnemyActor MonsterHit(const EnemyHitData& _HitData) : [Blown] ���� �������� ���� Ÿ���� �������Դϴ�.");
		}
		break;
	}
	case MonsterDamageType::Drilling:
	{
		static bool MsgCheck = false;
		if (false == MsgCheck)
		{
			MsgTextBox("BaseEnemyActor MonsterHit(const EnemyHitData& _HitData) : [Drilling] ���� �������� ���� Ÿ���� �������Դϴ�.");
		}
		break;
	}
	case MonsterDamageType::Launching:
	{
		static bool MsgCheck = false;
		if (false == MsgCheck)
		{
			MsgTextBox("BaseEnemyActor MonsterHit(const EnemyHitData& _HitData) : [Launching] ���� �������� ���� Ÿ���� �������Դϴ�.");
		}
		break;
	}
	case MonsterDamageType::Slam:
	{
		static bool MsgCheck = false;
		if (false == MsgCheck)
		{
			MsgTextBox("BaseEnemyActor MonsterHit(const EnemyHitData& _HitData) : [Slam] ���� �������� ���� Ÿ���� �������Դϴ�.");
		}
		break;
	}
	case MonsterDamageType::Downward:
	{
		static bool MsgCheck = false;
		if (false == MsgCheck)
		{
			MsgTextBox("BaseEnemyActor MonsterHit(const EnemyHitData& _HitData) : [Downward] ���� �������� ���� Ÿ���� �������Դϴ�.");
		}
		break;
	}
	case MonsterDamageType::Stun:
	{
		static bool MsgCheck = false;
		if (false == MsgCheck)
		{
			MsgTextBox("BaseEnemyActor MonsterHit(const EnemyHitData& _HitData) : [Stun] ���� �������� ���� Ÿ���� �������Դϴ�.");
		}
		break;
	}
	case MonsterDamageType::Hold:
	{
		static bool MsgCheck = false;
		if (false == MsgCheck)
		{
			MsgTextBox("BaseEnemyActor MonsterHit(const EnemyHitData& _HitData) : [Hold] ���� �������� ���� Ÿ���� �������Դϴ�.");
		}
		break;
	}
	default:
		MsgAssert("BaseEnemyActor MonsterHit(const EnemyHitData& _HitData) : �߸��� �ǰ� Ÿ���� ���Խ��ϴ�.");
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
	//Render����
	EnemyRenderer = CreateComponent<GameEngineFBXRenderer>();
	//PhysX(�浹)
	CapsulCol = CreateComponent<PhysXCapsuleComponent>();
	CapsulCol->SetPhysxMaterial(0, 0, 0);
	CapsulCol->CreatePhysXActors({100, 70, 100});
	CapsulCol->SetWorldPosition({ 0, 100, 0 });

	//���� ������ Enemy Collision
	MonsterCollision=CreateComponent<GameEngineCollision>(CollisionOrder::Enemy);
	//�ֺ� �÷��̾ �ν��ϴ� Collision(1ȸ��)
	RN_MonsterCollision = CreateComponent<GameEngineCollision>(CollisionOrder::RN_Enemy);
	//������ ���ݹ��� Collision
	MonsterAttackRange = CreateComponent<GameEngineCollision>(CollisionOrder::RN_Enemy);
	//ForWardCollision
	ForWardCollision = CreateComponent<GameEngineCollision>(CollisionOrder::RN_Enemy);
	ForWardCollision->GetTransform()->SetWorldScale({50,500,4000});
	ForWardCollision->SetColType(ColType::OBBBOX3D);
	
	//�ʱ�ȭ
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

void BaseEnemyActor::ChasePlayer(float _DeltaTime)
{
	//�������� Player�� vector�� ���������. �� ���͸� ���鼭 ����
	for (BasePlayerActor* Players : BasePlayerActor::GetPlayers())
	{
		float4 EnemyPosition = EnemyRenderer->GetTransform()->GetWorldPosition();
		float4 PlayerPosition = Players->GetTransform()->GetWorldPosition();
		CapsulCol->SetMove((PlayerPosition - EnemyPosition));
		//�ڿ������� ������
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
