#include "PrecompileHeader.h"
#include "NeroItemGlass.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineFontRenderer.h>
#include "UIFBXRenderer.h"
#include "PlayerActor_Nero.h"
#include <GameEngineCore/GameEngineFontRenderer.h>
bool NeroItemGlass::AddItemValue = false;
bool NeroItemGlass::DestroyItemValue = false;

NeroItemGlass::NeroItemGlass()
{
}

NeroItemGlass::~NeroItemGlass()
{
}

void NeroItemGlass::ArmRenderOff()
{
	ItemText->Off();
	CurCount->Off();
	SlashText->Off();
	MaxCount->Off();
	for (int i = 0; i < Arms.size(); i++)
	{
		Arms[i]->Off();
	}
	Ratio = 100.0f;
}

void NeroItemGlass::ArmRenderOn()
{
	ItemText->On();
	CurCount->On();
	SlashText->On();
	MaxCount->On();
	for (int i = 0; i < Arms.size(); i++)
	{
		Arms[i]->On();
	}
	Ratio = 1.0f;
}

void NeroItemGlass::Start()
{
	float Ratio = GameEngineWindow::GetScreenSize().y / 900;
	GetTransform()->SetLocalScale(float4::ONE * Ratio);

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
	SetItemText();
	//NeroUI_ItemGlass = UIFBXActorBase::CreateUIFBX(NeroUI_ItemGlass, { 560.0f,-380.0f,172.0f }, { 0.6f,0.6f,0.6f }, { -90.0f,0.0f,0.0f }, "NeroItemGlass.FBX","FBX_Alpha");
	ArmList = PlayerActor_Nero::GetBreakerListPointer();
}

void NeroItemGlass::Update(float _DeltaTime)
{
	MaxCount->SetText(std::to_string(MaxItem));
	CurCount->SetText(std::to_string(ArmList->size()-1));
	CurDevilBreaker = ArmList->back();
	AddDestroyValue();
	AddMoveBreaker(_DeltaTime);
	DestroyMoveBreaker(_DeltaTime);
}

void NeroItemGlass::SetItemText()
{
	ItemText = CreateComponent<GameEngineFontRenderer>(999);
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

void NeroItemGlass::AddDestroyValue()
{
	if (AddItemValue == true)
	{
		switch (CurDevilBreaker)
		{
		case DevilBreaker::None:
			ItemText->SetText("Donthave");
			AddItemValue = false;
			AddFirst = true;
			break;
		case DevilBreaker::Overture:
			ItemText->SetText("Overture");
			Render = CreateComponent<UIFBXRenderer>(3);
			Render->SetFBXMesh("OvertureArmUI.FBX", "FBX_Low");
			Arms.insert(Arms.begin(), Render);
			AddItemValue = false;
			AddFirst = true;
			break;
		case DevilBreaker::Gerbera:
			ItemText->SetText("Gerbera");
			Render = CreateComponent<UIFBXRenderer>(3);
			Render->SetFBXMesh("GerberaArmUI.FBX", "FBX_Low");
			Arms.insert(Arms.begin(), Render);
			AddItemValue = false;
			AddFirst = true;
			break;
		case DevilBreaker::BusterArm:
			ItemText->SetText("BusterArm");
			Render = CreateComponent<UIFBXRenderer>(3);
			Render->SetFBXMesh("BusterArmUI.FBX", "FBX_Low");
			Arms.insert(Arms.begin(), Render);
			AddItemValue = false;
			AddFirst = true;
			break;
		default:
			break;
		}
	}
	if (DestroyItemValue == true)
	{
		switch (CurDevilBreaker)
		{
		case DevilBreaker::None:
			ItemText->SetText("Donthave");
			DestroyItemValue = false;
			DestroyFirst = true;
			break;
		case DevilBreaker::Overture:
			ItemText->SetText("Overture");
			DestroyItemValue = false;
			DestroyFirst = true;
			break;
		case DevilBreaker::Gerbera:
			ItemText->SetText("Gerbera");
			DestroyItemValue = false;
			DestroyFirst = true;
			break;
		case DevilBreaker::BusterArm:
			ItemText->SetText("BusterArm");
			DestroyItemValue = false;
			DestroyFirst = true;
			break;
		default:
			break;
		}
	}
}


float4 NeroItemGlass::BezierTransform(const float4& _Start, const float4& _Height, const float4& _End, float _Ratio)
{
	float4 M0 = float4::LerpClamp(_Start, _Height, _Ratio);
	float4 M1 = float4::LerpClamp(_Height, _End, _Ratio);
	float4 Pos = float4::LerpClamp(M0, M1, _Ratio);

	return Pos;
}
void NeroItemGlass::AddMoveBreaker(float _Delta)
{

	if (AddFirst  == true)
	{
		Arms[0]->On();
		Time += _Delta * Ratio;
		Arms[0]->On();
		Arms[0]->GetTransform()->SetLocalScale(ThirdScale);
		Arms[0]->GetTransform()->SetLocalRotation(float4::LerpClamp({ 180.0f,0.0f,120.0f }, { 180.0f,0.0f,150.0f }, Time * 2.0f));
		Arms[0]->GetTransform()->SetLocalPosition(EndPos);
		if (Arms.size() >= 2)
		{
			Arms[1]->GetTransform()->SetLocalPosition(float4::LerpClamp(StartPos, EndPos, Time * 2.0f));
			Arms[1]->GetTransform()->SetLocalScale(float4::LerpClamp(FirstScale, ThirdScale, Time * 2.0f));
			Arms[1]->GetTransform()->SetLocalRotation(float4::LerpClamp({ 90.0f,0.0f,180.0f }, { 180.0f,0.0f,180.0f }, Time * 2.0f));
			if (Arms.size() >= 3)
			{
				Arms[2]->GetTransform()->SetLocalPosition(float4::LerpClamp(StartPos, EndPos, Time * 2.0f));
				Arms[2]->GetTransform()->SetLocalScale(float4::LerpClamp(SecondScale, ThirdScale, Time * 2.0f));
				Arms[2]->GetTransform()->SetLocalRotation(float4::LerpClamp({ 150.0f,00.0f,210.0f }, { 180.0f,00.0f,210.0f }, Time * 2.0f));
				if (Arms.size() >= 4)
				{
					Arms[3]->GetTransform()->SetLocalPosition(float4::LerpClamp(StartPos, EndPos, Time * 2.0f));
					Arms[3]->GetTransform()->SetLocalScale(float4::LerpClamp(ThirdScale, ThirdScale, Time * 2.0f));
					Arms[3]->GetTransform()->SetLocalRotation(float4::LerpClamp({ 210.0f,00.0f,240.0f }, { 210.0f,00.0f,255.0f }, Time * 2.0f));

				}
			}
		}

		if (AddFirst  == true && Time > 0.5f)
		{
			AddSecound  = true;
			Time = 0.0f;
			AddFirst  = false;
		}
	}
	if (AddSecound  == true)
	{

		Time += _Delta* Ratio;
		Arms[0]->GetTransform()->SetLocalPosition(float4::LerpClamp(EndPos, StartPos, Time * 2.0f));
		Arms[0]->GetTransform()->SetLocalScale(float4::LerpClamp(ThirdScale, FirstScale, Time * 2.0f));
		Arms[0]->GetTransform()->SetLocalRotation(float4::LerpClamp({ 180.0f,0.0f,150.0f }, { 90.0f,0.0f,180.0f }, Time * 2.0f));
		if (Arms.size() >= 2)
		{
			Arms[1]->GetTransform()->SetLocalPosition(float4::LerpClamp(EndPos, StartPos, Time * 2.0f));
			Arms[1]->GetTransform()->SetLocalScale(float4::LerpClamp(ThirdScale, SecondScale, Time * 2.0f));
			Arms[1]->GetTransform()->SetLocalRotation(float4::LerpClamp({ 180.0f,0.0f,180.0f }, { 150.0f,00.0f,210.0f }, Time * 2.0f));
			if (Arms.size() >= 3)
			{
				Arms[2]->GetTransform()->SetLocalPosition(float4::LerpClamp(EndPos, StartPos, Time * 2.0f));
				Arms[2]->GetTransform()->SetLocalScale(float4::LerpClamp(ThirdScale, ThirdScale, Time * 2.0f));
				Arms[2]->GetTransform()->SetLocalRotation(float4::LerpClamp({ 180.0f,00.0f,210.0f }, { 210.0f,00.0f,240.0f }, Time * 2.0f));
				if (Arms.size() >= 4)
				{
					Arms[3]->GetTransform()->SetLocalPosition(float4::LerpClamp(EndPos, { 741.0f,-270.0f,50.0f }, Time * 2.0f));
					Arms[3]->GetTransform()->SetLocalScale(float4::LerpClamp(ThirdScale, ThirdScale, Time * 2.0f));
					Arms[3]->GetTransform()->SetLocalRotation(float4::LerpClamp({ 210.0f,00.0f,255.0f }, { 210.0f,00.0f,320.0f }, Time * 2.0f));

				}
			}
		}
		if (Time >= 0.8f)
		{
			AddSecound  = false;
			Time = 0.0f;
		}
	}
	
}

void NeroItemGlass::DestroyMoveBreaker(float _Delta)
{
	//»Ñ¼ÅÁö´Â È¿°ú ÈÄ 
	if (DestroyFirst == true)
	{
		Arms[0]->SetMulColor(float4(0.0f, 0.0f, 0.0f, 1.0f));
		Time += _Delta;
		Arms[0]->GetTransform()->SetLocalScale(FirstScale);
		Arms[0]->GetTransform()->SetLocalRotation(float4::LerpClamp({ 90.0f,0.0f,180.0f }, { 90.0f,0.0f,180.0f }, Time * 2.0f));
		Arms[0]->GetTransform()->SetLocalPosition(BezierTransform(StartPos, { 640.0f,-300.0f,50.0f }, { 640.0f,-350.0f,50.0f }, Time * 2.0f));
		if (Arms.size() >= 2)
		{
			Arms[1]->GetTransform()->SetLocalPosition(float4::LerpClamp(StartPos, EndPos, Time * 2.0f));
			Arms[1]->GetTransform()->SetLocalScale(float4::LerpClamp(SecondScale, SecondScale, Time * 2.0f));
			Arms[1]->GetTransform()->SetLocalRotation(float4::LerpClamp({ 150.0f,00.0f,210.0f }, { 180.0f,00.0f,180.0f }, Time * 2.0f));
			if (Arms.size() >= 3)
			{
				Arms[2]->GetTransform()->SetLocalPosition(float4::LerpClamp(StartPos, EndPos, Time * 2.0f));
				Arms[2]->GetTransform()->SetLocalScale(float4::LerpClamp(ThirdScale, ThirdScale, Time * 2.0f));
				Arms[2]->GetTransform()->SetLocalRotation(float4::LerpClamp( { 210.0f,00.0f,240.0f }, { 180.0f,00.0f,210.0f }, Time * 2.0f));
				if (Arms.size() >= 4)
				{
					Arms[3]->GetTransform()->SetLocalPosition(float4::LerpClamp(StartPos, EndPos, Time * 2.0f));
					Arms[3]->GetTransform()->SetLocalScale(float4::LerpClamp(ThirdScale, ThirdScale, Time * 2.0f));
					Arms[3]->GetTransform()->SetLocalRotation(float4::LerpClamp({ 210.0f,00.0f,320.0f }, { 210.0f,00.0f,255.0f }, Time * 2.0f));
				
				}
			}
		}

		if (DestroyFirst == true && Time > 0.5f)
		{
			DestroyFirst = false;
			DestroySecond = true;
			Time = 0.0f;
		}
	}
	if (DestroySecond == true)
	{

		Time += _Delta;
		Arms[0]->Death();
		if (Arms.size() >= 2)
		{
			Arms[1]->GetTransform()->SetLocalPosition(float4::LerpClamp(EndPos, StartPos, Time * 2.0f));
			Arms[1]->GetTransform()->SetLocalScale(float4::LerpClamp(SecondScale, FirstScale, Time * 2.0f));
			Arms[1]->GetTransform()->SetLocalRotation(float4::LerpClamp({ 180.0f,00.0f,180.0f }, { 90.0f,0.0f,180.0f }, Time * 2.0f));
			if (Arms.size() >= 3)
			{
				Arms[2]->GetTransform()->SetLocalPosition(float4::LerpClamp(EndPos, StartPos, Time * 2.0f));
				Arms[2]->GetTransform()->SetLocalScale(float4::LerpClamp(ThirdScale, SecondScale, Time * 2.0f));
				Arms[2]->GetTransform()->SetLocalRotation(float4::LerpClamp({ 180.0f,00.0f,210.0f }, { 150.0f,00.0f,210.0f }, Time * 2.0f));
				if (Arms.size() >= 4)
				{
					Arms[3]->GetTransform()->SetLocalPosition(float4::LerpClamp(EndPos, StartPos, Time * 2.0f));
					Arms[3]->GetTransform()->SetLocalScale(float4::LerpClamp(ThirdScale, ThirdScale, Time * 2.0f));
					Arms[3]->GetTransform()->SetLocalRotation(float4::LerpClamp({ 210.0f,00.0f,255.0f }, { 210.0f,00.0f,240.0f }, Time * 2.0f));
				
				}
			}
		}
		if (Time >= 0.8f)
		{
			DestroySecond = false;
			Arms.erase(Arms.begin());
			Time = 0.0f;
		}
	}

}
