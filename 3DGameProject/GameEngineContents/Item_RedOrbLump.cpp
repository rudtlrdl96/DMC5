#include "PrecompileHeader.h"
#include "Item_RedOrbLump.h"
#include <GameEngineCore/GameEngineFBXMesh.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineCore/PhysXCapsuleComponent.h>
#include "FXSystem.h"
#include "Item_RedOrb.h"
Item_RedOrbLump::Item_RedOrbLump()
{
}

Item_RedOrbLump::~Item_RedOrbLump()
{
}

void Item_RedOrbLump::Start()
{
	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Map");
		NewDir.Move("RedOrb");

		if (nullptr == GameEngineFBXMesh::Find("sm0917_redorbhaisyutsu.meshout.FBX"))
		{
			GameEngineFBXMesh::Load(NewDir.GetPlusFileName("sm0917_redorbhaisyutsu.meshout.FBX").GetFullPath());
		}
	}
	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Sound");
		NewDir.Move("SFX");
		NewDir.Move("RedOrb");

		if (nullptr == GameEngineSound::Find("RedOrb_0.wav"))
		{
			GameEngineSound::Load(NewDir.GetPlusFileName("RedOrb_0.wav").GetFullPath());
			GameEngineSound::Load(NewDir.GetPlusFileName("RedOrb_1.wav").GetFullPath());
			GameEngineSound::Load(NewDir.GetPlusFileName("RedOrb_2.wav").GetFullPath());
			GameEngineSound::Load(NewDir.GetPlusFileName("RedOrb_3.wav").GetFullPath());
		}
	}

	Effect = CreateComponent<FXSystem>();
	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Effect");
		NewDir.Move("RedOrb");

		if (nullptr == FXData::Find("RedOrbLump_Disappear.effect"))
		{
			FXData::Load(NewDir.GetPlusFileName("RedOrbLump_Disappear.effect").GetFullPath());
		}
		Effect->CreateFX(FXData::Find("RedOrbLump_Disappear.effect"));
	}

	FBXMesh = CreateComponent<GameEngineFBXRenderer>();
	FBXMesh->SetFBXMesh("sm0917_redorbhaisyutsu.meshout.FBX", "FBX");
	FBXMesh->GetTransform()->SetLocalScale(float4::ONE * 1.5f);

	Col = CreateComponent<GameEngineCollision>(CollisionOrder::RedOrbLump);
	Col->SetColType(ColType::AABBBOX3D);
	Col->GetTransform()->SetLocalPosition({ 0, 100, 0 });
	Col->GetTransform()->SetLocalScale({ 250, 250, 250 });


	Orbs.resize(36);
	for (int i = 0; i < 8; i++)
	{
		Orbs[i] = GetLevel()->CreateActor<Item_RedOrb>();
		Orbs[i]->SetWait();
		Orbs[i]->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition());
		Orbs[i]->GetTransform()->SetParent(GetTransform());
		float Random = GameEngineRandom::MainRandom.RandomFloat(-1, 1);
		Orbs[i]->GetTransform()->SetLocalRotation(float4::UP * (static_cast<float>(45 * i) + Random * 40.0f));
	}
	for (int i = 8; i < 36; i++)
	{
		Orbs[i] = GetLevel()->CreateActor<Item_RedOrb>();
		Orbs[i]->SetWait();
		Orbs[i]->GetTransform()->SetParent(GetTransform());
		float Random = GameEngineRandom::MainRandom.RandomFloat(0, 1);
		Orbs[i]->GetTransform()->SetLocalRotation(float4::UP * Random * 360.0f);
	}

	PhysX = CreateComponent<PhysXCapsuleComponent>();
	PhysX->SetPhysxMaterial(1.0f, 1.0f, 0.05f);
	const float4& AcScale = GetTransform()->GetLocalScale();
	const float4& MeshScale = FBXMesh->GetMeshScale();
	physx::PxVec3 PxScale = { MeshScale.x * AcScale.x, MeshScale.y * AcScale.y, MeshScale.z * AcScale.z };
	PhysX->SetObstacleObject();
	PhysX->CreatePhysXActors(PxScale);
	PhysX->TurnOffGravity();
}

void Item_RedOrbLump::Update(float _DeltaTime)
{
	if (GetLiveTime() < 0.12f) { return; }
	if (nullptr != Col->Collision(CollisionOrder::PlayerAttack))
	{
		ResetLiveTime();
		HP--;
		if (HP == 0)
		{
			Break();
			Col->Off();
			return;
		}
		Damage();
	}
}

void Item_RedOrbLump::Damage()
{
	for (int i = 8 + HP * 4 - 1; i >= 8 + (HP - 1) * 4; i--)
	{
		Orbs[i]->GetTransform()->SetParent(nullptr);
		Orbs[i]->On();
	}
	switch (GameEngineRandom::MainRandom.RandomInt(0, 2))
	{
	case 0 :
		GameEngineSound::Play("RedOrb_0.wav");
		break;
	case 1:
		GameEngineSound::Play("RedOrb_1.wav");
		break;
	case 2:
	default:
		GameEngineSound::Play("RedOrb_2.wav");
		break;
	}
}

void Item_RedOrbLump::Break()
{
	for (int i = 0; i < 8; i++)
	{
		Orbs[i]->GetTransform()->SetParent(nullptr);
		Orbs[i]->On();
	}
	Effect->PlayFX("RedOrbLump_Disappear.effect");
	FBXMesh->GetRenderUnit(1, 0)->Off();
	PhysX->Off();
	GameEngineSound::Play("RedOrb_3.wav");
}
