#include "PrecompileHeader.h"
#include "Item_DevilBreaker.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>
#include "EffectRenderer.h"
Item_DevilBreaker::Item_DevilBreaker() 
{
}

Item_DevilBreaker::~Item_DevilBreaker() 
{
}

void Item_DevilBreaker::SetDevilBreaker(DevilBreaker _Value)
{
	Type = _Value;
	IconRenderer->RectInit("Effect_2D");
	IconRenderer->LockRotation();
	switch (_Value)
	{
	case DevilBreaker::None:
		break;
	case DevilBreaker::Overture:
		ArmRenderer->SetFBXMesh("OvertureArmUI.FBX", "FBX");
		IconRenderer->SetTexture("DiffuseTexture", "ItemExPlane_OverTure.png");
		break;
	case DevilBreaker::Gerbera:
		ArmRenderer->SetFBXMesh("GerberaArmUI.FBX", "FBX");
		IconRenderer->SetTexture("DiffuseTexture", "ItemExPlane_Gerbera.png");
		break;
	case DevilBreaker::BusterArm:
		ArmRenderer->SetFBXMesh("BusterArmUI.FBX", "FBX");
		IconRenderer->SetTexture("DiffuseTexture", "ItemExPlane_BusterArm.png");
		break;
	default:
		break;
	}
	IconRenderer->GetTransform()->SetWorldScale({ 268, 268, 268 });


}

DevilBreaker Item_DevilBreaker::Take()
{
	Off();
	return Type;
}

void Item_DevilBreaker::Start()
{
	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Mesh");
		NewDir.Move("UIMesh");
		NewDir.Move("NeroItemMesh");

		if (nullptr == GameEngineFBXMesh::Find("NeroItemGlass.FBX"))
		{
			std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".fbx" });
			for (GameEngineFile File : Files)
			{
				GameEngineFBXMesh::Load(File.GetFullPath());
			}
		}
	}

	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Texture");
		NewDir.Move("UI");
		NewDir.Move("MainMenuTexture");
		NewDir.Move("PlayLevelTexture");

		if (nullptr == GameEngineTexture::Find("ItemExPlane_OverTure.png"))
		{
			std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".fbx" });
			for (GameEngineFile File : Files)
			{
				GameEngineTexture::Load(File.GetFullPath());
			}
		}
	}

	ArmRenderer = CreateComponent<GameEngineFBXRenderer>();
	ArmRenderer->GetTransform()->SetLocalScale({ 1.5f, 1.5f, 1.5f});
	IconRenderer = CreateComponent<EffectRenderer>();
	IconRenderer->GetTransform()->SetLocalPosition({ 75, 275 });
	IconRenderer->GetTransform()->SetLocalScale({ 200, 200, 200 });
	IconRenderer->GetTransform()->SetWorldRotation(float4::ZERO);

	Col = CreateComponent<GameEngineCollision>(CollisionOrder::DevilBreaker);
	Col->GetTransform()->SetLocalPosition({ 75, 0, 0 });
	Col->GetTransform()->SetLocalScale({ 150, 150, 150 });
}

void Item_DevilBreaker::Update(float _DeltaTime)
{
}
