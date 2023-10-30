#include "PrecompileHeader.h"
#include "NeroLoading.h"
#include "InvenUIButton.h"
#include "InfoUIRender.h"
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineFontRenderer.h>
#include "FXSystem.h"

NeroLoading::NeroLoading() 
{
}

NeroLoading::~NeroLoading() 
{
}

void NeroLoading::Start()
{
	LodingEffect = CreateComponent<FXSystem>();
	LodingEffect->GetTransform()->SetLocalScale({ 1000.0f,0.0f,0.0f });
	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Effect");
	NewDir.Move("EffectUI");
	std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".effect" });
	for (GameEngineFile File : Files)
	{
		if (nullptr == FXData::Find(File.GetFileName()))
		{
			FXData::Load(File.GetFullPath());
		}
		LodingEffect->CreateFX(FXData::Find(File.GetFileName()));
	}
	LodingEffect->PlayFX("LodingBar.effect");
	LodingEffect->Loop = true;
	GetTransform()->SetLocalScale(GetTransform()->GetLocalScale() * GameEngineActor::ScreenRatio);
	LodingBackRender = CreateComponent<GameEngineUIRenderer>(0);
	LodingBackRender->SetTexture("LoadingTexture.png");
	LodingBackRender->GetTransform()->SetLocalScale(Screenscale);
	LodingBackRender->GetTransform()->SetLocalPosition({ 0.0f,0.0f,0.0f });

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
	GaugeFrontRender->ImageClippingX(CurLoading, ClipXDir::Left);
	TextSetting();
	BlinkLodingBar(_Delta);
}

void NeroLoading::TextStart()
{
	FirstLine = CreateComponent<GameEngineFontRenderer>(5);
	FirstLine->SetFont("나눔바른고딕 옛한글");
	FirstLine->SetFontFlag(FW1_LEFT);
	FirstLine->SetScale(20);
	FirstLine->SetColor(float4(0.48f,0.556f,0.623f));
	FirstLine->GetTransform()->SetLocalPosition({ 141.0f,80.f,0.0f });

	SecoundLine = CreateComponent<GameEngineFontRenderer>(5);
	SecoundLine->SetFont("나눔바른고딕 옛한글");
	SecoundLine->SetFontFlag(FW1_LEFT);
	SecoundLine->SetScale(20);
	SecoundLine->SetColor(float4(0.48f, 0.556f, 0.623f));
	SecoundLine->GetTransform()->SetLocalPosition({ 141.0f,47.5f,0.0f });

	ThirdLine = CreateComponent<GameEngineFontRenderer>(5);
	ThirdLine->SetFont("나눔바른고딕 옛한글");
	ThirdLine->SetFontFlag(FW1_LEFT);
	ThirdLine->SetScale(20);
	ThirdLine->SetColor(float4(0.48f, 0.556f, 0.623f));
	ThirdLine->GetTransform()->SetLocalPosition({ 141.0f,14.0f,0.0f });

	ForthLine = CreateComponent<GameEngineFontRenderer>(5);
	ForthLine->SetFont("나눔바른고딕 옛한글");
	ForthLine->SetFontFlag(FW1_LEFT);
	ForthLine->SetScale(20);
	ForthLine->SetColor(float4(0.48f, 0.556f, 0.623f));
	ForthLine->GetTransform()->SetLocalPosition({ 141.0f,-19.0f,0.0f });

	FifthLine = CreateComponent<GameEngineFontRenderer>(5);
	FifthLine->SetFont("나눔바른고딕 옛한글");
	FifthLine->SetFontFlag(FW1_LEFT);
	FifthLine->SetScale(20);
	FifthLine->SetColor(float4(0.48f, 0.556f, 0.623f));
	FifthLine->GetTransform()->SetLocalPosition({ 141.0f,-52.0f,0.0f });
	
		
}

void NeroLoading::TextSetting()
{
	if (Index == 0)
	{
		FirstLine->SetText("게임 내 튜토리얼 팁은 일지 중지 메뉴에서");
		SecoundLine->SetText("언제든지 볼 수 있습니다. 혹시 놓쳤을지");
		ThirdLine->SetText("모르는 게임 정보를 확인해 보십시오!");
		ForthLine->SetText("");
		FifthLine->SetText("");
	}
	else
	{
		FirstLine->SetText("콤보를 깔끔하게 연결하고 싶다면 자동");
		SecoundLine->SetText("지원을 사용해 보십시오. 같은 공격 명령을");
		ThirdLine->SetText("사용하여 쉽게 스타일리시한 기술을 펼칠 수");
		ForthLine->SetText("있습니다. 자동 지원은 게임 플레이 중에");
		FifthLine->SetText("ALT를 홀드해 켜고 끌 수 있습니다.");
	}
}


void NeroLoading::BlinkLodingBar(float _Delta)
{
	if (UpValue == true)
	{
		AddTime += _Delta;
		float Ratio = GameEngineMath::LerpLimit(0.5, 1.0f, AddTime * 0.5f);
		GaugeFrontRender->BSCControl(0.5, Ratio, 0.5f);
		if (Ratio >= 1.0f)
		{
			UpValue = false;
			DownValue = true;
			AddTime = 0.0f;
		}
	}
	if (DownValue == true)
	{
		AddTime += _Delta;
		float Ratio = GameEngineMath::LerpLimit(1.0, 0.5f, AddTime*0.5f);
		GaugeFrontRender->BSCControl(0.5, Ratio, 0.5f);
		if (Ratio <= 0.5f)
		{
			UpValue = true;
			DownValue = false;
			AddTime = 0.0f;
		}
	}
}

