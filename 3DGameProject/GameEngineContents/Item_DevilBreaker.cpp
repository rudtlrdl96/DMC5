#include "PrecompileHeader.h"
#include "Item_DevilBreaker.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>
#include "EffectRenderer.h"
#include "Char_ChoiceUI.h"
Item_DevilBreaker::Item_DevilBreaker() 
{
}

Item_DevilBreaker::~Item_DevilBreaker() 
{
}

void Item_DevilBreaker::SetDevilBreaker(DevilBreaker _Value)
{
	if (Char_ChoiceUI::GetPlayerType() == ChoicePlayerType::VERGIL)
	{
		return;
	}
	Type = _Value;
	IconRenderer->RectInit("Effect_2D");
	IconRenderer->LockRotation();
	switch (_Value)
	{
	case DevilBreaker::None:
		break;
	case DevilBreaker::Overture:
		FBXMesh->SetFBXMesh("OvertureArmUI.FBX", "FBX");
		IconRenderer->SetTexture("DiffuseTexture", "ItemExPlane_OverTure.png");
		break;
	case DevilBreaker::Gerbera:
		FBXMesh->SetFBXMesh("GerberaArmUI.FBX", "FBX");
		IconRenderer->SetTexture("DiffuseTexture", "ItemExPlane_Gerbera.png");
		break;
	case DevilBreaker::BusterArm:
		FBXMesh->SetFBXMesh("BusterArmUI.FBX", "FBX");
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
	if (Char_ChoiceUI::GetPlayerType() == ChoicePlayerType::VERGIL)
	{
		Off();
		return;
	}

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

	FBXMesh = CreateComponent<GameEngineFBXRenderer>();
	FBXMesh->GetTransform()->SetLocalScale({ 1.5f, 1.5f, 1.5f});
	FBXMesh->SetBaseColor({1.0f,0.f,0.f,0.f});
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
