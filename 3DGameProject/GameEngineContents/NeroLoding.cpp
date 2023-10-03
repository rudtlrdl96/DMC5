#include "PrecompileHeader.h"
#include "NeroLoding.h"
#include "InvenUIButton.h"
#include "InfoUIRender.h"
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineFontRenderer.h>

NeroLoding::NeroLoding() 
{
}

NeroLoding::~NeroLoding() 
{
}

void NeroLoding::Start()
{
	LodingBackRender = CreateComponent<GameEngineUIRenderer>(0);
	LodingBackRender->SetTexture("LodingTexture.png");
	LodingBackRender->GetTransform()->SetLocalScale({ 1920.0f,1080.0f,0.0f });
	LodingBackRender->GetTransform()->SetLocalPosition({ 155.0f,0.0f,0.0f });

	ExplanePtr = GetLevel()->CreateActor<InfoUIRender>();
	ExplanePtr->GetRender()->SetTexture("NullTexture.png");
	ExplanePtr->GetRender_Bar()->SetTexture("LodingExplaneBar.png");
	ExplanePtr->GetRender_Bar()->GetTransform()->SetLocalPosition({ 392.0f,96.0f,0.0f });
	ExplanePtr->GetRender_Bar()->GetTransform()->SetLocalScale({ -546.0f,4.0f,0.0f });
	ExplanePtr->GetRender_Base()->SetTexture("NullTexture.png");
	ExplanePtr->GetText()->SetScale(42.0f);
	ExplanePtr->GetText()->SetColor({ 0.305f, 0.96f, 0.94f, 1.0f });
	ExplanePtr->GetText()->GetTransform()->SetLocalPosition({164.0f,147.0f,0.0f});
	ExplanePtr->SetUIText({ ._Text = "TIPS" });
	
	ArrowLeftPtr = GetLevel()->CreateActor<InvenUIButton>();
	ArrowLeftPtr->GetRender()->SetTexture("LodingArrow.png");
	ArrowLeftPtr->GetRender()->GetTransform()->SetLocalScale({ 18.0f,19.0f,0.0f });
	ArrowLeftPtr->SetLerpValue(float4(90.f, 0.f, 0.0f), float4(80.f, 0.f, 0.0f));
	ArrowLeftPtr->SetEvent([this]()
		{
			if (Index == 1)
			{
				Index = 0;
			}
			else
			{
				Index++;
			}

		});
	ArrowRightPtr = GetLevel()->CreateActor<InvenUIButton>();
	ArrowRightPtr->GetRender()->SetTexture("LodingArrow.png");
	ArrowRightPtr->GetRender()->GetTransform()->SetLocalScale({ -18.0f,19.0f,0.0f });
	ArrowRightPtr->SetLerpValue(float4(685.f, 0.f, 0.0f), float4(695.f, 0.f, 0.0f));
	ArrowRightPtr->SetEvent([this]()
		{
			if (Index == 0)
			{
				Index = 1;
			}
			else
			{
				Index--;
			}
		});
	TextStart();
}

void NeroLoding::Update(float _Delta)
{
	if (Index == 0)
	{
		FirstLine->SetText("무슨 말을 적는게 좋을까요?");
		SecoundLine->SetText("아! 하고 싶은 말이 있어요");
		ThirdLine->SetText("옆에 화살표를 눌러주시겠어요?");
	}
	else
	{
		FirstLine->SetText("우리팀 사랑해요");
		SecoundLine->SetText("우리 모두 힘내봐여");
		ThirdLine->SetText("더 열심히 할게요");
	}
}

void NeroLoding::TextStart()
{
	FirstLine = CreateComponent<GameEngineFontRenderer>(5);
	FirstLine->SetFont("나눔바른고딕 옛한글");
	FirstLine->SetFontFlag(FW1_LEFT);
	FirstLine->SetScale(24);
	FirstLine->SetColor(float4(0.48f,0.556f,0.623f));
	FirstLine->GetTransform()->SetLocalPosition({ 141.0f,80.f,0.0f });

	SecoundLine = CreateComponent<GameEngineFontRenderer>(5);
	SecoundLine->SetFont("나눔바른고딕 옛한글");
	SecoundLine->SetFontFlag(FW1_LEFT);
	SecoundLine->SetScale(24);
	SecoundLine->SetColor(float4(0.48f, 0.556f, 0.623f));
	SecoundLine->GetTransform()->SetLocalPosition({ 141.0f,47.5f,0.0f });
	ThirdLine = CreateComponent<GameEngineFontRenderer>(5);
	ThirdLine->SetFont("나눔바른고딕 옛한글");
	ThirdLine->SetFontFlag(FW1_LEFT);
	ThirdLine->SetScale(24);
	ThirdLine->SetColor(float4(0.48f, 0.556f, 0.623f));
	ThirdLine->GetTransform()->SetLocalPosition({ 141.0f,15.0f,0.0f });

}

