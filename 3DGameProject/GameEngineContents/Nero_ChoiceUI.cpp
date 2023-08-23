#include "PrecompileHeader.h"
#include "Nero_ChoiceUI.h"
#include "UIEnums.h"
#include "UIFontRender.h"
Nero_ChoiceUI::Nero_ChoiceUI()
{
}

Nero_ChoiceUI::~Nero_ChoiceUI()
{
}

void Nero_ChoiceUI::Start()
{


	Nero_ChoiceUIRender = CreateComponent<GameEngineUIRenderer>(1);
	Nero_ChoiceUIRender->GetTransform()->SetLocalScale(Scale);
	Nero_ChoiceUIRender->SetTexture("KakaoTalk_20230822_162007017.png");
	Nero_ChoiceUIRender->GetTransform()->SetLocalPosition(Pos);

	Nero_ChoiceUIRender2 = CreateComponent<GameEngineUIRenderer>(2);
	Nero_ChoiceUIRender2->GetTransform()->SetLocalScale(Scale);
	Nero_ChoiceUIRender2->SetTexture("Char_SelectBase.png");

}


void Nero_ChoiceUI::Update(float _DeltaTime)
{
	if (GameEngineInput::IsDown("UIDEBUGMODE"))
	{
		TestIndex++;
	} 
	if (TestIndex % 2 == 0)
	{
		ScaleDownTime = 0.0f;
		ScaleUpTime += _DeltaTime;
		float4 LerpUpScale = float4::LerpClamp(Scale, SeletScale, ScaleUpTime * 7.0f);
		Nero_ChoiceUIRender->On();
		Nero_ChoiceUIRender->GetTransform()->SetLocalScale(LerpUpScale);
		Nero_ChoiceUIRender2->Off();
		Nero_ChoiceUIRender2->GetTransform()->SetLocalScale(Scale);

	}
	else
	{
		ScaleUpTime = 0.0f; 
		ScaleDownTime += _DeltaTime;
		Nero_ChoiceUIRender->Off();
		Nero_ChoiceUIRender->GetTransform()->SetLocalScale(Scale);
		float4 LerpDownScale = float4::LerpClamp(SeletScale, Scale, ScaleDownTime * 7.0f);
		Nero_ChoiceUIRender2->GetTransform()->SetLocalScale(LerpDownScale);

		Nero_ChoiceUIRender2->On();
	}
}

