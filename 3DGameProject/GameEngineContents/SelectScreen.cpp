#include "PrecompileHeader.h"
#include "SelectScreen.h"
#include "ContentButton.h"

#include <GameEngineCore/GameEngineUIRenderer.h>
SelectScreen::SelectScreen() 
{
}

SelectScreen::~SelectScreen() 
{
}
void SelectScreen::Start()
{
	//OFF가 안되서 SELECT이미지를 NULL로
	ContinueButton = GetLevel()->CreateActor<ContentButton>();
	ContinueButton->GetTransform()->SetLocalPosition({ 0.0f,0.0f,0.0f });
	ContinueButton->GetRender()->SetTexture("TestText.png");
	ContinueButton->GetRender()->GetTransform()->SetLocalScale({ 300.0f,40.0f,0.0f });
	ContinueButton->GetRender()->GetTransform()->SetLocalPosition({ 0.0f,-200.0f,0.0f });
	ContinueButton->GetRender_Select()->SetTexture("NullTexture.png");
	ContinueButton->GetRender_Select()->GetTransform()->SetLocalPosition({ 0.0f,-200.0f,0.0f });
	ContinueButton->GetRender_Select()->GetTransform()->SetLocalScale({ 1600.0f,50.0f,0.0f });

	OptionButton = GetLevel()->CreateActor<ContentButton>();
	OptionButton->GetTransform()->SetLocalPosition({ 0.0f,0.0f,0.0f });
	OptionButton->GetRender()->SetTexture("TestText.png");
	OptionButton->GetRender()->GetTransform()->SetLocalScale({ 300.0f,40.0f,0.0f });
	OptionButton->GetRender()->GetTransform()->SetLocalPosition({ 0.0f,-250.0f,0.0f });
	OptionButton->GetRender_Select()->SetTexture("NullTexture.png");
	OptionButton->GetRender_Select()->GetTransform()->SetLocalPosition({ 0.0f,-250.0f,0.0f });
	OptionButton->GetRender_Select()->GetTransform()->SetLocalScale({ 1600.0f,50.0f,0.0f });

	ExitButton = GetLevel()->CreateActor<ContentButton>();
	ExitButton->GetTransform()->SetLocalPosition({ 0.0f,0.0f,0.0f });
	ExitButton->GetRender()->GetTransform()->SetLocalScale({ 300.0f,40.0f,0.0f });
	ExitButton->GetRender()->GetTransform()->SetLocalPosition({ 0.0f,-300.0f,0.0f });
	ExitButton->GetRender()->SetTexture("TestText.png");
	ExitButton->GetRender_Select()->SetTexture("NullTexture.png");
	ExitButton->GetRender_Select()->GetTransform()->SetLocalPosition({ 0.0f,-300.0f,0.0f });
	ExitButton->GetRender_Select()->GetTransform()->SetLocalScale({ 1600.0f,50.0f,0.0f });

}

void SelectScreen::Update(float _DeltaTime)
{
}

