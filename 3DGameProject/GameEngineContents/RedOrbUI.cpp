#include "PrecompileHeader.h"
#include "RedOrbUI.h"
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineFontRenderer.h>
int RedOrbUI::RedOrbValue = 0;
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
	RedOrbRender->GetTransform()->SetLocalScale({ 43.0f,60.0f,0.0f });
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
	RedOrbFont->SetText(std::to_string(RedOrbValue));
	if (GameEngineInput::IsUp("UI_OrbCheat"))
	{
		RedOrbValue += 1000;
	}
}

