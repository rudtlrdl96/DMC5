#include "PrecompileHeader.h"
#include "RedOrbUI.h"
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineFontRenderer.h>
bool RedOrbUI::IsRedOrbAni = false;
int RedOrbUI::RedOrbValue = 400;
RedOrbUI::RedOrbUI() 
{
}

RedOrbUI::~RedOrbUI() 
{
}

void RedOrbUI::Start()
{
	GameEngineDirectory RedOrbDir;
	RedOrbDir.MoveParentToDirectory("ContentResources");
	RedOrbDir.Move("ContentResources");
	RedOrbDir.Move("Texture");
	RedOrbDir.Move("UI");
	RedOrbDir.Move("PlayLevelUI");
	if (nullptr == GameEngineSprite::Find("RedOrbNumAni.png"))
	{
		GameEngineSprite::LoadSheet(RedOrbDir.GetPlusFileName("RedOrbNumAni.png").GetFullPath(), 1, 11);
	}
	if (nullptr == GameEngineTexture::Find("RedOrb.png"))
	{
		GameEngineTexture::Load(RedOrbDir.GetPlusFileName("RedOrb.png").GetFullPath());
	}
	RedOrbRender = CreateComponent<GameEngineUIRenderer>(0);
	RedOrbRender->SetTexture("RedOrb.png");
	RedOrbRender->GetTransform()->SetLocalScale(Scale);
	RedOrbRender->GetTransform()->SetLocalPosition({ 550.0f,380.0f,-50.0f });

	RedOrbFont = CreateComponent<GameEngineFontRenderer>(0);
	RedOrbFont->SetFont("DMC5Font");
	RedOrbFont->SetScale(50.0f);
	RedOrbFont->SetColor(float4::WHITE);
	RedOrbFont->SetFontFlag(FW1_TEXT_FLAG::FW1_LEFT);
	RedOrbFont->GetTransform()->SetLocalPosition({ 582.0f,407.0f,-50.0f });
	AniCreate();
}

void RedOrbUI::Update(float _Delta)
{
	RedOrbFont->SetText(std::to_string(RedOrbValue));
	if (GameEngineInput::IsUp("UI_OrbCheat"))
	{
		RedOrbValue += 5000;
		IsRedOrbAni = true;
	}
	if (IsRedOrbAni == true)
	{
		IsOff = true;
		
	}
	if (IsOff == true)
	{
		AniTime += _Delta;
		RedOrbFont->Off();
		if (AniTime > 0.33f)
		{
			AniTime = 0.0f;
			RedOrbFont->On();
			IsOff = false;
		}
	}
	UpdateOrb(_Delta);

}

void RedOrbUI::AniCreate()
{

	Num_Ani1 = CreateComponent<GameEngineUIRenderer>(3);
	Num_Ani1->SetTexture("NullTexture.png");
	Num_Ani1->CreateAnimation({ .AnimationName = "NumAni1", .SpriteName = "RedOrbNumAni.png", .Start = 0, .End = 10,.FrameInter = 0.03f, .Loop = false });
	Num_Ani1->GetTransform()->SetLocalScale(AniScale);
	Num_Ani1->GetTransform()->SetLocalPosition({ 685.0f,375.0f,0.0f });

	Num_Ani2 = CreateComponent<GameEngineUIRenderer>(3);
	Num_Ani2->SetTexture("NullTexture.png");
	Num_Ani2->CreateAnimation({ .AnimationName = "NumAni2", .SpriteName = "RedOrbNumAni.png", .Start = 0, .End = 10,.FrameInter = 0.03f, .Loop = false });
	Num_Ani2->GetTransform()->SetLocalScale(AniScale);
	Num_Ani2->GetTransform()->SetLocalPosition({ 666.0f,375.0f,0.0f });

	Num_Ani3 = CreateComponent<GameEngineUIRenderer>(3);
	Num_Ani3->SetTexture("NullTexture.png");
	Num_Ani3->CreateAnimation({ .AnimationName = "NumAni3", .SpriteName = "RedOrbNumAni.png", .Start = 0, .End = 10,.FrameInter = 0.03f, .Loop = false });
	Num_Ani3->GetTransform()->SetLocalScale(AniScale);
	Num_Ani3->GetTransform()->SetLocalPosition({ 647.0f,375.0f,0.0f });

	Num_Ani4 = CreateComponent<GameEngineUIRenderer>(3);
	Num_Ani4->SetTexture("NullTexture.png");
	Num_Ani4->CreateAnimation({ .AnimationName = "NumAni4", .SpriteName = "RedOrbNumAni.png", .Start = 0, .End = 10,.FrameInter = 0.03f, .Loop = false });
	Num_Ani4->GetTransform()->SetLocalScale(AniScale);
	Num_Ani4->GetTransform()->SetLocalPosition({ 626.0f,375.0f,0.0f });

	Num_Ani5 = CreateComponent<GameEngineUIRenderer>(3);
	Num_Ani5->SetTexture("NullTexture.png");
	Num_Ani5->CreateAnimation({ .AnimationName = "NumAni5", .SpriteName = "RedOrbNumAni.png", .Start = 0, .End = 10,.FrameInter = 0.03f, .Loop = false });
	Num_Ani5->GetTransform()->SetLocalScale(AniScale);
	Num_Ani5->GetTransform()->SetLocalPosition({ 605.0f,375.0f,0.0f });

	Num_Ani6 = CreateComponent<GameEngineUIRenderer>(3);
	Num_Ani6->SetTexture("NullTexture.png");
	Num_Ani6->CreateAnimation({ .AnimationName = "NumAni6", .SpriteName = "RedOrbNumAni.png", .Start = 0, .End = 10,.FrameInter = 0.03f, .Loop = false });
	Num_Ani6->GetTransform()->SetLocalScale(AniScale);
	Num_Ani6->GetTransform()->SetLocalPosition({ 587.0f,375.0f,0.0f });


}

void RedOrbUI::UpdateOrb(float _Delta)
{
	if (IsRedOrbAni == true)
	{
		if (RedOrbValue >= 100000)
		{
			AniTime += _Delta;
			Num_Ani1->GetTransform()->SetLocalPosition({ 685.0f,375.0f,0.0f });
			Num_Ani2->GetTransform()->SetLocalPosition({ 666.0f,375.0f,0.0f });
			Num_Ani3->GetTransform()->SetLocalPosition({ 647.0f,375.0f,0.0f });
			Num_Ani4->GetTransform()->SetLocalPosition({ 626.0f,375.0f,0.0f });
			Num_Ani5->GetTransform()->SetLocalPosition({ 605.0f,375.0f,0.0f });
			Num_Ani6->GetTransform()->SetLocalPosition({ 587.0f,375.0f,0.0f });
			Num_Ani1->ChangeAnimation("NumAni1");
			Num_Ani2->ChangeAnimation("NumAni2");
			Num_Ani3->ChangeAnimation("NumAni3");
			Num_Ani4->ChangeAnimation("NumAni4");
			Num_Ani5->ChangeAnimation("NumAni5");
			Num_Ani6->ChangeAnimation("NumAni6");
			IsRedOrbAni = false;
			
		}
		else if (RedOrbValue <= 100000 && RedOrbValue >= 10000)
		{
			Num_Ani1->GetTransform()->SetLocalPosition({ 675.0f,375.0f,0.0f });
			Num_Ani2->GetTransform()->SetLocalPosition({ 654.0f,375.0f,0.0f });
			Num_Ani3->GetTransform()->SetLocalPosition({ 634.0f,375.0f,0.0f });
			Num_Ani4->GetTransform()->SetLocalPosition({ 614.0f,375.0f,0.0f });
			Num_Ani5->GetTransform()->SetLocalPosition({ 594.0f,375.0f,0.0f });
			Num_Ani1->ChangeAnimation("NumAni1");
			Num_Ani2->ChangeAnimation("NumAni2");
			Num_Ani3->ChangeAnimation("NumAni3");
			Num_Ani4->ChangeAnimation("NumAni4");
			Num_Ani5->ChangeAnimation("NumAni5");
			IsRedOrbAni = false;
		}
		else if (RedOrbValue <= 10000 && RedOrbValue >= 1000)
		{
			Num_Ani1->GetTransform()->SetLocalPosition({ 657.0f,375.0f,0.0f });
			Num_Ani2->GetTransform()->SetLocalPosition({ 635.0f,375.0f,0.0f });
			Num_Ani3->GetTransform()->SetLocalPosition({ 617.0f,375.0f,0.0f });
			Num_Ani4->GetTransform()->SetLocalPosition({ 595.0f,375.0f,0.0f });
			Num_Ani1->ChangeAnimation("NumAni1");
			Num_Ani2->ChangeAnimation("NumAni2");
			Num_Ani3->ChangeAnimation("NumAni3");
			Num_Ani4->ChangeAnimation("NumAni4");
			IsRedOrbAni = false;

		}
		else if (RedOrbValue <= 1000 && RedOrbValue >= 100)
		{
			Num_Ani1->GetTransform()->SetLocalPosition({ 638.0f,375.0f,0.0f });
			Num_Ani2->GetTransform()->SetLocalPosition({ 618.0f,375.0f,0.0f });
			Num_Ani3->GetTransform()->SetLocalPosition({ 593.0f,375.0f,0.0f });
			Num_Ani1->ChangeAnimation("NumAni1");
			Num_Ani2->ChangeAnimation("NumAni2");
			Num_Ani3->ChangeAnimation("NumAni3");
			IsRedOrbAni = false;
		}
	}
	

}

