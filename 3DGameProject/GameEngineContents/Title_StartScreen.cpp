#include "PrecompileHeader.h"
#include "UIEnums.h"
#include "Title_StartScreen.h"
#include "Title_SelectScreen.h"
#include <GameEngineCore/GameEngineFontRenderer.h>
#include <GameEngineCore/GameEngineLevel.h>
#include "NetworkGUI.h"
Title_StartScreen::Title_StartScreen()
{
}

Title_StartScreen::~Title_StartScreen()
{
}

void Title_StartScreen::Start()
{
	SkyColor = { 0.42f,0.73f,0.75f,SkyColor_Alpha };
	SelectText = CreateComponent<GameEngineFontRenderer>(2);
	SelectText->GetTransform()->SetLocalPosition(Pos);
	SelectText->SetFont("DMC5Font");
	SelectText->SetFontFlag(FW1_LEFT);
	SelectText->SetScale(32);
	SelectText->SetText("Select Host OR Client");
}

void Title_StartScreen::Update(float _DeltaTime)
{
	Blink(_DeltaTime);
	NetworkGUI::GetInst()->SetEntryCallBack([this]()
		{
			Death();
			std::shared_ptr<Title_SelectScreen> Title_SelectScreenPtr = GetLevel()->CreateActor<Title_SelectScreen>();
			Title_SelectScreenPtr->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
			Title_SelectScreenPtr->GetTransform()->SetLocalPosition({ 0.0f,0.0f,0.0f });
		});
}

void Title_StartScreen::Blink(float _DeltaTime)
{
	if (SwichValue == false && SkyColor_Alpha <= 1.0f)
	{
		SkyColor_Alpha -= 0.5f* _DeltaTime;
		if (SkyColor_Alpha < 0.6f)
		{
			SwichValue = true;
		}
	}
	if (SwichValue == true)
	{
		SkyColor_Alpha += 0.5f * _DeltaTime;
		if (SkyColor_Alpha >= 1.0f)
		{
			SkyColor_Alpha = 1.0f;
			SwichValue = false;
		}
	}
	SelectText->SetColor({ SkyColor.r,SkyColor.g,SkyColor.b,SkyColor_Alpha });
}

