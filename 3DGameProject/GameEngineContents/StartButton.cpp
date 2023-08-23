#include "PrecompileHeader.h"
#include "UIEnums.h"
#include "StartButton.h"
#include "SelectScreen.h"

#include <GameEngineCore/GameEngineLevel.h>
StartButton::StartButton() 
{
}

StartButton::~StartButton() 
{
}

void StartButton::Start()
{
	StartButtonPtr = CreateComponent<GameEngineUIRenderer>(UIOrder::SELECT);
	StartButtonPtr->GetTransform()->SetLocalPosition(Pos);
	StartButtonPtr->SetScaleToTexture("PressAnyKey.png");

	ButtonCol = CreateComponent<GameEngineCollision>(UICollisonOrder::UI);
	ButtonCol->GetTransform()->SetLocalPosition(Pos);
	ButtonCol->GetTransform()->SetLocalScale(Scale);
	ButtonCol->SetColType(ColType::AABBBOX2D);
}

void StartButton::Update(float _DeltaTime)
{
	Time += _DeltaTime;
	Blink(_DeltaTime);
	if (ButtonCol->Collision(UICollisonOrder::Mouse, ColType::AABBBOX2D, ColType::AABBBOX2D))
	{
		//货肺款 竣磐 积己.
		if (GameEngineInput::IsUp("CLICK"))
		{
			Death();
			std::shared_ptr<SelectScreen> SelectScreenPtr = GetLevel()->CreateActor<SelectScreen>();
			SelectScreenPtr->GetTransform()->SetLocalScale({ 1.0f, 1.0f, 1.0f });
			SelectScreenPtr->GetTransform()->SetLocalPosition({ 0.0f,0.0f,0.0f });
		}

	}
}

void StartButton::Blink(float _DeltaTime)
{
	if (SwichValue == false && StartButtonPtr->ColorOptionValue.MulColor.a <= 1)
	{
		StartButtonPtr->ColorOptionValue.MulColor.a -= 0.5f * _DeltaTime;
		if (StartButtonPtr->ColorOptionValue.MulColor.a < 0.5f)
		{
			SwichValue = true;
		}
	}
	if (SwichValue == true)
	{
		StartButtonPtr->ColorOptionValue.MulColor.a += 0.5f * _DeltaTime;
		if (StartButtonPtr->ColorOptionValue.MulColor.a >= 1.0f)
		{
			StartButtonPtr->ColorOptionValue.MulColor.a = 1.0f;
			SwichValue = false;
		}
	}
}

