#include "PrecompileHeader.h"
#include "NeroItemGlass.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineFontRenderer.h>
#include "UIFBXRenderer.h"
#include "PlayerActor_Nero.h"
#include <GameEngineCore/GameEngineFontRenderer.h>

NeroItemGlass::NeroItemGlass()
{
}

NeroItemGlass::~NeroItemGlass()
{
}

void NeroItemGlass::Start()
{
	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Mesh");
	NewDir.Move("UIMesh");
	NewDir.Move("NeroUIMesh");
	NewDir.Move("NeroItemMesh");

	if (nullptr == GameEngineFBXMesh::Find("NeroItemGlass.FBX"))
	{
		std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".fbx" });
		for (GameEngineFile File : Files)
		{
			GameEngineFBXMesh::Load(File.GetFullPath());
		}
	}
	NeroUI_ItemGlass = UIFBXActorBase::CreateUIFBX(NeroUI_ItemGlass, { 560.0f,-380.0f,172.0f }, { 0.6f,0.6f,0.6f }, { -90.0f,0.0f,0.0f }, "NeroItemGlass.FBX");

	NeroUI_Overture = UIFBXActorBase::CreateUIFBX(NeroUI_ItemGlass, { 730.0f,-300.0f,50.0f }, {5.0f,5.0f,5.0f }, { 89.0f,0.0f,179.0f }, "OvertureArmUI.FBX", "FBX");
	NeroUI_Gerbera = UIFBXActorBase::CreateUIFBX(NeroUI_ItemGlass, { 730.0f,-300.0f,50.0f }, {4.0f,4.0f,4.0f }, { 160.0f,0.0f,211.0f }, "GerberaArmUI.FBX", "FBX");
	NeroUI_BusterArm = UIFBXActorBase::CreateUIFBX(NeroUI_ItemGlass, { 730.0f,-300.0f,-50.0f }, {3.0f,3.0f,3.0f }, { 230.0f,0.0f,250.0f }, "BusterArmUI.FBX", "FBX");

	SetItemText();
}

void NeroItemGlass::Update(float _DeltaTime)
{
	MaxCount->SetText(std::to_string(MaxItem));
	CurCount->SetText(std::to_string(PlayerActor_Nero::GetBreakerStackPointer()->size()));
	switch (CurDevilBreaker)
	{
	case DevilBreaker::None:
		ItemText->SetText("Donthave");
		break;
	case DevilBreaker::Overture:
		ItemText->SetText("Overture");
		break;
	case DevilBreaker::Gerbera:
		ItemText->SetText("Gerbera");
		break;
	case DevilBreaker::BusterArm:
		ItemText->SetText("BusterArm");
		break;
	default:
		break;
	}
}

void NeroItemGlass::SetItemText()
{
	ItemText = CreateComponent<GameEngineFontRenderer>(5);
	ItemText->SetFont("DMC5Font");
	ItemText->SetFontFlag(FW1_CENTER);
	ItemText->SetScale(36);
	ItemText->SetColor(float4(0.701f, 0.772f, 0.788f, 1.0f));
	ItemText->GetTransform()->SetLocalPosition({ 540.0f,-340.f,0.0f });


	CurCount = CreateComponent<GameEngineFontRenderer>(5);
	CurCount->SetFont("DMC5Font");
	CurCount->SetFontFlag(FW1_CENTER);
	CurCount->SetScale(50);
	CurCount->SetColor(float4(0.701f, 0.772f, 0.788f, 1.0f));
	CurCount->GetTransform()->SetLocalPosition({ 634.0f,-327.f,0.0f, });


	SlashText = CreateComponent<GameEngineFontRenderer>(8);
	SlashText->SetFont("DMC5Font");
	SlashText->SetFontFlag(FW1_CENTER);
	SlashText->SetScale(36);
	SlashText->SetColor(float4(0.656f, 0.668f, 0.665f, 1.0f));
	SlashText->GetTransform()->SetLocalPosition({ 660.0f,-340.f,0.0f });
	SlashText->SetText("/");

	MaxCount = CreateComponent<GameEngineFontRenderer>(8);
	MaxCount->SetFont("DMC5Font");
	MaxCount->SetFontFlag(FW1_CENTER);
	MaxCount->SetScale(36);
	MaxCount->SetColor(float4(0.656f, 0.668f, 0.665f, 1.0f));
	MaxCount->GetTransform()->SetLocalPosition({ 680.0f,-340.f,0.0f });
}

