#include "PrecompileHeader.h"
#include "BaseEnemyActor.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>

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
		MsgAssert("BaseEnemyActor MonsterHit(const EnemyHitData& _HitData) : �ǰ� Ÿ���� �������� �ʾҽ��ϴ�.");
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
		static bool MsgCheck = false;
		if (false == MsgCheck)
		{
			MsgTextBox("BaseEnemyActor MonsterHit(const EnemyHitData& _HitData) : [Air] ���� �������� ���� Ÿ���� �������Դϴ�.");
		}
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

void BaseEnemyActor::Start()
{
	EnemyRenderer = CreateComponent<GameEngineFBXRenderer>();

	EnemyMeshLoad();
	EnemyTextureLoad();
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


void BaseEnemyActor::SpuerArmorOn()
{
	IsSpuerArmorValue = true;

	if (nullptr != SpuerArmorOn_Callback)
	{
		SpuerArmorOn_Callback();
	}
}

void BaseEnemyActor::SuperArmorOff()
{
	IsSuperArmorValue = false;

	if (nullptr != SpuerArmorOff_Callback)
	{
		SpuerArmorOff_Callback();
	}
}