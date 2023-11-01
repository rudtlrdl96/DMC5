#include "PrecompileHeader.h"
#include "RedOrbUI.h"
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineFontRenderer.h>
int RedOrbUI::PrevOrb = false;
int RedOrbUI::RedOrbValue = 100;
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
}

void RedOrbUI::Update(float _Delta)
{
	if (RedOrbNum != RedOrbValue)
	{
		AddTime += _Delta;
		RedOrbNum = GameEngineMath::LerpLimit(static_cast<float>(PrevOrb), static_cast<float>(RedOrbValue), AddTime*1.5f);
		RedOrbFont->SetText(std::to_string(static_cast<int>(RedOrbNum)));
	}
	else
	{
		AddTime = 0.0f;
	}
	if (GameEngineInput::IsUp("UI_OrbCheat"))
	{
		PrevOrb = RedOrbValue;
		RedOrbValue += 5000;
	}
	
}

