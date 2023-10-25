#include "PrecompileHeader.h"
#include "Item_RedOrbLump.h"
#include <GameEngineCore/GameEngineFBXMesh.h>
#include <GameEngineCore/GameEngineCollision.h>
#include "FXSystem.h"
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

	Effect = CreateComponent<FXSystem>();
	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Effect");
		NewDir.Move("RedOrb");

		if (nullptr == FXData::Find("RedOrb_Disappear.effect"))
		{
			FXData::Load(NewDir.GetPlusFileName("RedOrb_Disappear.effect").GetFullPath());
		}
		Effect->CreateFX(FXData::Find("RedOrb_Disappear.effect"));
	}

	FBXMesh = CreateComponent<GameEngineFBXRenderer>();
	FBXMesh->SetFBXMesh("sm0917_redorbhaisyutsu.meshout.FBX", "FBX");
	FBXMesh->GetTransform()->SetLocalScale(float4::ONE * 2.0f);

	Col = CreateComponent<GameEngineCollision>(CollisionOrder::RedOrbLump);
	Col->SetColType(ColType::AABBBOX3D);
	Col->GetTransform()->SetLocalPosition({ 0, 100, 0 });
	Col->GetTransform()->SetLocalScale({ 250, 250, 250 });

}

void Item_RedOrbLump::Update(float _DeltaTime)
{
}
