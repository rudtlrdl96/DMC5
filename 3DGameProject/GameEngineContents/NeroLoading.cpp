#include "PrecompileHeader.h"
#include "NeroLoading.h"
#include "InvenUIButton.h"
#include "InfoUIRender.h"
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineFontRenderer.h>

NeroLoading::NeroLoading() 
{
}

NeroLoading::~NeroLoading() 
{
}

void NeroLoading::Start()
{
	GetTransform()->SetLocalScale(GetTransform()->GetLocalScale() * GameEngineActor::ScreenRatio);
	LodingBackRender = CreateComponent<GameEngineUIRenderer>(0);
	LodingBackRender->SetTexture("LoadingTexture.png");
	LodingBackRender->GetTransform()->SetLocalScale({ 1920.0f,1080.0f,0.0f });
	LodingBackRender->GetTransform()->SetLocalPosition({ 155.0f,0.0f,0.0f });

	GaugeBackRender = CreateComponent<GameEngineUIRenderer>(2);
	GaugeBackRender->SetTexture("LoadingGaugeBar.png");
	GaugeBackRender->GetTransform()->SetLocalPosition({ 0.0f,-400.0f,0.0f });
	GaugeBackRender->GetTransform()->SetLocalScale({ 1090.0f,15.0f,0.0f });

	GaugeFrontRender = CreateComponent<GameEngineUIRenderer>(2);
	GaugeFrontRender->SetTexture("LoadingGaugeFrontBar.png");
	GaugeFrontRender->GetTransform()->SetLocalPosition({ 0.0f,-400.0f,0.0f });
	GaugeFrontRender->GetTransform()->SetLocalScale({ 1090.0f,15.0f,0.0f });
	GaugeFrontRender->ImageClippingX(0.0f, ClipXDir::Left);
	
	ExplanePtr = GetLevel()->CreateActor<InfoUIRender>();
	ExplanePtr->GetRender()->SetTexture("NullTexture.png");
	ExplanePtr->GetRender_Bar()->SetTexture("LoadingExplaneBar.png");
	ExplanePtr->GetRender_Bar()->GetTransform()->SetLocalPosition({ 392.0f,96.0f,0.0f });
	ExplanePtr->GetRender_Bar()->GetTransform()->SetLocalScale({ -546.0f,4.0f,0.0f });
	ExplanePtr->GetRender_Base()->SetTexture("NullTexture.png");
	ExplanePtr->GetText()->SetScale(42.0f);
	ExplanePtr->GetText()->SetColor({ 0.305f, 0.96f, 0.94f, 1.0f });
	ExplanePtr->GetText()->GetTransform()->SetLocalPosition({164.0f,147.0f,0.0f});
	ExplanePtr->SetUIText({ ._Text = "TIPS" });
	
	ArrowLeftPtr = GetLevel()->CreateActor<InvenUIButton>();
	ArrowLeftPtr->GetRender()->SetTexture("LoadingArrow.png");
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
				CurLoading += 10;
				Index++;
			}

		});
	ArrowRightPtr = GetLevel()->CreateActor<InvenUIButton>();
	ArrowRightPtr->GetRender()->SetTexture("LoadingArrow.png");
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
				CurLoading -= 10;
				Index--;
			}
		});
	TextStart();
}

void NeroLoading::Update(float _Delta)
{
	// æ≤∑πµÂ ¡¯«‡∑¸ = CurLoading
	GaugeFrontRender->ImageClippingX(CurLoading, ClipXDir::Left);
	TextSetting();

}

void NeroLoading::TextStart()
{
	FirstLine = CreateComponent<GameEngineFontRenderer>(5);
	FirstLine->SetFont("≥™¥ÆπŸ∏•∞ÌµÒ øæ«—±€");
	FirstLine->SetFontFlag(FW1_LEFT);
	FirstLine->SetScale(24);
	FirstLine->SetColor(float4(0.48f,0.556f,0.623f));
	FirstLine->GetTransform()->SetLocalPosition({ 141.0f,80.f,0.0f });

	SecoundLine = CreateComponent<GameEngineFontRenderer>(5);
	SecoundLine->SetFont("≥™¥ÆπŸ∏•∞ÌµÒ øæ«—±€");
	SecoundLine->SetFontFlag(FW1_LEFT);
	SecoundLine->SetScale(24);
	SecoundLine->SetColor(float4(0.48f, 0.556f, 0.623f));
	SecoundLine->GetTransform()->SetLocalPosition({ 141.0f,47.5f,0.0f });
	ThirdLine = CreateComponent<GameEngineFontRenderer>(5);
	ThirdLine->SetFont("≥™¥ÆπŸ∏•∞ÌµÒ øæ«—±€");
	ThirdLine->SetFontFlag(FW1_LEFT);
	ThirdLine->SetScale(24);
	ThirdLine->SetColor(float4(0.48f, 0.556f, 0.623f));
	ThirdLine->GetTransform()->SetLocalPosition({ 141.0f,15.0f,0.0f });

}

void NeroLoading::TextSetting()
{
	if (Index == 0)
	{
		FirstLine->SetText("º≥∏Ì∂ıø° π´Ωº∏ª¿ª ≥÷¿ª±Ó");
		SecoundLine->SetText("±◊≥… ¿Œ∞‘¿” √≥∑≥ «“±Ó");
		ThirdLine->SetText("»Ï...±∏¡∂∞° ¿Ã∞‘ ∏¬≥™");
	}
	else
	{
		FirstLine->SetText("1¡÷¿œ ≥≤æ“¥Ÿ");
		SecoundLine->SetText("≈∏¥⁄..≈∏¥⁄..≈∏¥⁄");
		ThirdLine->SetText("¿Ã∞‹æﬂ∏∏ «—¥Ÿ..");
	}
}

