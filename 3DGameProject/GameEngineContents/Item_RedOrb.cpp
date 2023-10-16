#include "PrecompileHeader.h"
#include "Item_RedOrb.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>
#include "EffectRenderer.h"
#include "FXSystem.h"
Item_RedOrb::Item_RedOrb()
{
}

Item_RedOrb::~Item_RedOrb()
{
}

void Item_RedOrb::Take()
{
	Renderer->Off();
	Col->Off();
	Effect->PlayFX("RedOrb_Disappear.effect");

	GetLevel()->TimeEvent.AddEvent(0.5f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager) {
		Off();
		Renderer->On();
		Col->On();
		});
}

void Item_RedOrb::Start()
{
	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Map");
		NewDir.Move("RedOrb");

		if (nullptr == GameEngineFBXMesh::Find("orb000_0_low.meshout.FBX"))
		{
			GameEngineFBXMesh::Load(NewDir.GetPlusFileName("orb000_0_low.meshout.FBX").GetFullPath());
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

	Renderer = CreateComponent<GameEngineFBXRenderer>();
	Renderer->SetFBXMesh("orb000_0_low.meshout.FBX", "FBX");

	Col = CreateComponent<GameEngineCollision>(CollisionOrder::RedOrb);
	Col->GetTransform()->SetLocalScale({ 30, 30, 30 });

	GetTransform()->SetLocalScale({ 2.f, 2.f, 2.f });
}

void Item_RedOrb::Update(float _DeltaTime)
{
}
