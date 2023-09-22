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
	//0번째 로테이션
	NeroUI_Overture0 = UIFBXActorBase::CreateUIFBX(NeroUI_Overture0, { 635.0f,-300.0f,50.0f }, { 3.0f,3.0f,3.0f }, { 180.0f,0.0f,150.0f }, "OvertureArmUI.FBX", "FBX");
	NeroUI_Overture0->Off();
	//1번째 로테이션
	NeroUI_Overture = UIFBXActorBase::CreateUIFBX(NeroUI_Overture, { 730.0f,-300.0f,50.0f }, {5.0f,5.0f,5.0f }, { 90.0f,0.0f,180.0f }, "OvertureArmUI.FBX", "FBX");
	//2번째 로테이션
	NeroUI_Gerbera = UIFBXActorBase::CreateUIFBX(NeroUI_Gerbera, { 730.0f,-300.0f,50.0f }, {4.0f,4.0f,4.0f }, { 150.0f,30.0f,210.0f }, "GerberaArmUI.FBX", "FBX");
	//3번째 로테이션
	NeroUI_BusterArm = UIFBXActorBase::CreateUIFBX(NeroUI_BusterArm, { 730.0f,-300.0f,50.0f }, { 3.0f,3.0f,3.0f }, { 210.0f,00.0f,240.0f }, "BusterArmUI.FBX", "FBX");
	ArmList = PlayerActor_Nero::GetBreakerListPointer();
	SetItemText();
}

void NeroItemGlass::Update(float _DeltaTime)
{
	MaxCount->SetText(std::to_string(MaxItem));
	CurCount->SetText(std::to_string(ArmList->size()));
	CurDevilBreaker = ArmList->back();
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
	MoveItem(_DeltaTime);
	//테스트용 코드
	//데빌 브레이커를 주웠을 때 상황
	// 1단계 0번 1번 2번 인덱스의 데빌브레이커가 스케일이 줄며 90도로 회전한다 
	// 2단계 기울기를 조절한다.
	// 3단계 기울기 + 크기를 키운다.
	// 데빌브레이커가 추가가 되면 
	if (true == GameEngineInput::IsPress("UI_Tab"))
	{

		first = true;
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

void NeroItemGlass::MoveItem(float _Delta)
{
	if (first == true)
	{
		Time += _Delta;
		NeroUI_Overture0->Off();
		NeroUI_Overture0->GetTransform()->SetLocalScale({ 3.0f,3.0f,3.0f });
		NeroUI_Overture0->GetTransform()->SetLocalRotation({ 180.0f,0.0f,150.0f });
		NeroUI_Overture0->GetTransform()->SetLocalPosition({ 635.0f,-300.0f,50.0f });

		NeroUI_Overture->GetTransform()->SetLocalPosition(float4::LerpClamp({ 730.0f,-300.0f,50.0f }, { 635.0f,-300.0f,50.0f }, Time * 2.0f));
		NeroUI_Overture->GetTransform()->SetLocalScale(float4::LerpClamp({ 5.0f,5.0f,5.0f }, { 3.0f,3.0f,3.0f }, Time * 2.0f));
		NeroUI_Overture->GetTransform()->SetLocalRotation(float4::LerpClamp({ 90.0f,0.0f,180.0f }, { 180.0f,0.0f,180.0f }, Time * 2.0f));

		NeroUI_Gerbera->GetTransform()->SetLocalPosition(float4::LerpClamp({ 730.0f,-300.0f,50.0f }, { 635.0f,-300.0f,50.0f }, Time * 2.0f));
		NeroUI_Gerbera->GetTransform()->SetLocalScale(float4::LerpClamp({ 4.0f,4.0f,4.0f }, { 3.0f,3.0f,3.0f }, Time * 2.0f));
		NeroUI_Gerbera->GetTransform()->SetLocalRotation(float4::LerpClamp({ 150.0f,30.0f,210.0f }, { 180.0f,30.0f,210.0f }, Time * 2.0f));

		NeroUI_BusterArm->GetTransform()->SetLocalPosition(float4::LerpClamp({ 730.0f,-300.0f,50.0f }, { 730.0f,-300.0f,50.0f }, Time * 2.0f));
		NeroUI_BusterArm->GetTransform()->SetLocalScale({ 3.0f,3.0f,3.0f });
		NeroUI_BusterArm->GetTransform()->SetLocalRotation(float4::LerpClamp({ 210.0f,00.0f,240.0f }, { 210.0f,00.0f,300.0f }, Time * 2.0f));
		
		if (first == true && Time > 0.5f)
		{
			secound = true;
			Time = 0.0f;
			first = false;
		}

	}
	if (secound == true)
	{
		Time += _Delta;
		NeroUI_Overture0->On();
		NeroUI_Overture0->GetTransform()->SetLocalRotation(float4::LerpClamp({ 180.0f,0.0f,150.0f }, { 180.0f,0.0f,180.0f }, Time * 2.0f));
		NeroUI_Overture->GetTransform()->SetLocalRotation(float4::LerpClamp({ 180.0f,0.0f,180.0f }, { 180.0f,0.0f,210.0f }, Time * 2.0f));
		NeroUI_Gerbera->GetTransform()->SetLocalRotation(float4::LerpClamp({ 180.0f,30.0f,210.0f }, { 180.0f,00.0f,240.0f }, Time * 2.0f));

		if (Time >= 0.8f)
		{
			secound = false;
			third = true;
			Time = 0.0f;

		}
	}
	if (third == true)
	{
		
		Time += _Delta;
		NeroUI_Overture0->GetTransform()->SetLocalPosition(float4::LerpClamp({ 635.0f, -300.0f, 50.0f }, { 730.0f,-300.0f,50.0f }, Time * 2.0f));
		NeroUI_Overture0->GetTransform()->SetLocalScale(float4::LerpClamp({ 3.0f,3.0f,3.0f }, { 5.0f,5.0f,5.0f }, Time * 2.0f));
		NeroUI_Overture0->GetTransform()->SetLocalRotation(float4::LerpClamp({180.0f,0.0f,180.0f }, { 90.0f,0.0f,180.0f }, Time * 2.0f));

		NeroUI_Overture->GetTransform()->SetLocalPosition(float4::LerpClamp({ 635.0f, -300.0f, 50.0f }, { 730.0f,-300.0f,50.0f }, Time * 2.0f));
		NeroUI_Overture->GetTransform()->SetLocalScale(float4::LerpClamp({ 3.0f,3.0f,3.0f }, { 4.0f,4.0f,4.0f }, Time * 2.0f));
		NeroUI_Overture->GetTransform()->SetLocalRotation(float4::LerpClamp({ 180.0f,0.0f,210.0f }, { 150.0f,30.0f,210.0f }, Time * 2.0f));

		NeroUI_Gerbera->GetTransform()->SetLocalPosition(float4::LerpClamp({ 635.0f,-300.0f,50.0f }, { 730.0f,-300.0f,50.0f }, Time * 2.0f));
		NeroUI_Gerbera->GetTransform()->SetLocalScale(float4::LerpClamp({ 3.0f,3.0f,3.0f }, { 3.0f,3.0f,3.0f }, Time * 2.0f));
		NeroUI_Gerbera->GetTransform()->SetLocalRotation(float4::LerpClamp({ 180.0f,00.0f,240.0f }, { 210.0f,00.0f,240.0f }, Time * 2.0f));
		if (Time >= 0.8f)
		{
			third = false;
			Time = 0.0f;

		}
	}
}
