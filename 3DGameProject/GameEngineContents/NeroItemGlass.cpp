#include "PrecompileHeader.h"
#include "NeroItemGlass.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineFontRenderer.h>
#include "UIFBXRenderer.h"
#include "PlayerActor_Nero.h"
#include <GameEngineCore/GameEngineFontRenderer.h>
bool NeroItemGlass::AddItemValue = false;
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
	ArmList = PlayerActor_Nero::GetBreakerListPointer();
	SetItemText();
}

void NeroItemGlass::Update(float _DeltaTime)
{
	MaxCount->SetText(std::to_string(MaxItem));
	CurCount->SetText(std::to_string(Arms.size()));
	CurDevilBreaker = ArmList->back();
	if (AddItemValue == true)
	{
		switch (CurDevilBreaker)
		{
		case DevilBreaker::None:
			ItemText->SetText("Donthave");
			AddItemValue = false;
			first = true;
			break;
		case DevilBreaker::Overture:
			ItemText->SetText("Overture");
			Render = CreateComponent<UIFBXRenderer>(3);
			Render->SetFBXMesh("OvertureArmUI.FBX", "FBX_Low");
			Arms.insert(Arms.begin(), Render);
			AddItemValue = false;
			first = true;
			break;
		case DevilBreaker::Gerbera:
			ItemText->SetText("Gerbera");
			Render = CreateComponent<UIFBXRenderer>(3);
			Render->SetFBXMesh("GerberaArmUI.FBX", "FBX_Low");
			Arms.insert(Arms.begin(), Render);
			AddItemValue = false;
			first = true;
			break;
		case DevilBreaker::BusterArm:
			ItemText->SetText("BusterArm");
			Render = CreateComponent<UIFBXRenderer>(3);
			Render->SetFBXMesh("BusterArmUI.FBX", "FBX_Low");
			Arms.insert(Arms.begin(), Render);
			AddItemValue = false;
			first = true;
			break;
		default:
			break;
		}
	}
	
	MoveBreaker(_DeltaTime);
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


void NeroItemGlass::MoveBreaker(float _Delta)
{

	if (first == true)
	{
		Time += _Delta;
		Arms[0]->On();
		Arms[0]->GetTransform()->SetLocalScale({ 3.0f,3.0f,3.0f });
		Arms[0]->GetTransform()->SetLocalRotation(float4::LerpClamp({ 180.0f,0.0f,120.0f }, { 180.0f,0.0f,150.0f }, Time * 2.0f));
		Arms[0]->GetTransform()->SetLocalPosition({ 635.0f,-300.0f,50.0f });
		if (Arms.size() >= 2)
		{
			Arms[1]->GetTransform()->SetLocalPosition(float4::LerpClamp({ 730.0f,-300.0f,50.0f }, { 635.0f,-300.0f,50.0f }, Time * 2.0f));
			Arms[1]->GetTransform()->SetLocalScale(float4::LerpClamp({ 5.0f,5.0f,5.0f }, { 3.0f,3.0f,3.0f }, Time * 2.0f));
			Arms[1]->GetTransform()->SetLocalRotation(float4::LerpClamp({ 90.0f,0.0f,180.0f }, { 180.0f,0.0f,180.0f }, Time * 2.0f));
			if (Arms.size() >= 3)
			{
				Arms[2]->GetTransform()->SetLocalPosition(float4::LerpClamp({ 730.0f,-300.0f,50.0f }, { 635.0f,-300.0f,50.0f }, Time * 2.0f));
				Arms[2]->GetTransform()->SetLocalScale(float4::LerpClamp({ 4.0f,4.0f,4.0f }, { 3.0f,3.0f,3.0f }, Time * 2.0f));
				Arms[2]->GetTransform()->SetLocalRotation(float4::LerpClamp({ 150.0f,00.0f,210.0f }, { 180.0f,00.0f,210.0f }, Time * 2.0f));
				if (Arms.size() >= 4)
				{
					Arms[3]->GetTransform()->SetLocalPosition(float4::LerpClamp({ 730.0f,-300.0f,50.0f }, { 635.0f,-300.0f,50.0f }, Time * 2.0f));
					Arms[3]->GetTransform()->SetLocalScale(float4::LerpClamp({ 3.0f,3.0f,3.0f }, { 3.0f,3.0f,3.0f }, Time * 2.0f));
					Arms[3]->GetTransform()->SetLocalRotation(float4::LerpClamp({ 210.0f,00.0f,240.0f }, { 210.0f,00.0f,255.0f }, Time * 2.0f));

				}
			}
		}

		if (first == true && Time > 0.5f)
		{
			third = true;
			Time = 0.0f;
			first = false;
		}
	}
	if (third == true)
	{

		Time += _Delta;
		Arms[0]->GetTransform()->SetLocalPosition(float4::LerpClamp({ 635.0f, -300.0f, 50.0f }, { 730.0f,-300.0f,50.0f }, Time * 2.0f));
		Arms[0]->GetTransform()->SetLocalScale(float4::LerpClamp({ 3.0f,3.0f,3.0f }, { 5.0f,5.0f,5.0f }, Time * 2.0f));
		Arms[0]->GetTransform()->SetLocalRotation(float4::LerpClamp({ 180.0f,0.0f,150.0f }, { 90.0f,0.0f,180.0f }, Time * 2.0f));
		if (Arms.size() >= 2)
		{
			Arms[1]->GetTransform()->SetLocalPosition(float4::LerpClamp({ 635.0f, -300.0f, 50.0f }, { 730.0f,-300.0f,50.0f }, Time * 2.0f));
			Arms[1]->GetTransform()->SetLocalScale(float4::LerpClamp({ 3.0f,3.0f,3.0f }, { 4.0f,4.0f,4.0f }, Time * 2.0f));
			Arms[1]->GetTransform()->SetLocalRotation(float4::LerpClamp({ 180.0f,0.0f,180.0f }, { 150.0f,00.0f,210.0f }, Time * 2.0f));
			if (Arms.size() >= 3)
			{
				Arms[2]->GetTransform()->SetLocalPosition(float4::LerpClamp({ 635.0f, -300.0f, 50.0f }, { 730.0f,-300.0f,50.0f }, Time * 2.0f));
				Arms[2]->GetTransform()->SetLocalScale(float4::LerpClamp({ 3.0f,3.0f,3.0f }, { 3.0f,3.0f,3.0f }, Time * 2.0f));
				Arms[2]->GetTransform()->SetLocalRotation(float4::LerpClamp({ 180.0f,00.0f,210.0f }, { 210.0f,00.0f,240.0f }, Time * 2.0f));
				if (Arms.size() >= 4)
				{
					Arms[3]->GetTransform()->SetLocalPosition(float4::LerpClamp({ 635.0f,-300.0f,50.0f }, { 741.0f,-270.0f,50.0f }, Time * 2.0f));
					Arms[3]->GetTransform()->SetLocalScale(float4::LerpClamp({ 3.0f,3.0f,3.0f }, { 3.0f,3.0f,3.0f }, Time * 2.0f));
					Arms[3]->GetTransform()->SetLocalRotation(float4::LerpClamp({ 210.0f,00.0f,255.0f }, { 210.0f,00.0f,320.0f }, Time * 2.0f));

				}
			}
		}
		if (Time >= 0.8f)
		{
			third = false;
			Time = 0.0f;
		}
	}
	
}
