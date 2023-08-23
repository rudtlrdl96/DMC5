#include "PrecompileHeader.h"
#include "UIEnums.h"
#include "Title_StartScreen.h"
#include "Title_SelectScreen.h"
#include "UIFontRender.h"
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineFontRenderer.h>
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

	ButtonCol = CreateComponent<GameEngineCollision>(UICollisonOrder::UI);
	ButtonCol->GetTransform()->SetLocalPosition(Pos);
	ButtonCol->GetTransform()->SetLocalScale(Scale);
	ButtonCol->SetColType(ColType::AABBBOX2D);

}

void Title_StartScreen::Update(float _DeltaTime)
{
	Blink(_DeltaTime);
	if (ButtonCol->Collision(UICollisonOrder::Mouse, ColType::AABBBOX2D, ColType::AABBBOX2D))
	{
		//새로운 엑터 생성. 이때 GUI를 통해 함수 넣어주자.
		if (GameEngineInput::IsUp("CLICK"))
		{
			NewScreen();
		}

	}
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

void Title_StartScreen::NewScreen()
{
	Death();
	std::shared_ptr<Title_SelectScreen> Title_SelectScreenPtr = GetLevel()->CreateActor<Title_SelectScreen>();
	Title_SelectScreenPtr->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
	Title_SelectScreenPtr->GetTransform()->SetLocalPosition({ 0.0f,0.0f,0.0f });
}

