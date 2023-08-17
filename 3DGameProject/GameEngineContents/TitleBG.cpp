#include "PrecompileHeader.h"
#include "TitleBG.h"
#include "UIFontRender.h"
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>
TitleBG::TitleBG() 
{
}

TitleBG::~TitleBG() 
{
}

void TitleBG::Start()
{
	std::shared_ptr<GameEngineCamera> Camera = GetLevel()->GetCamera(100);
	Camera->SetProjectionType(CameraType::Orthogonal);
	Camera->GetTransform()->SetLocalPosition({ 0, 0, -1000.0f });

	TitleBGRender = CreateComponent<GameEngineUIRenderer>(1);
	TitleBGRender->GetTransform()->SetLocalScale(Scale);
	TitleBGRender->SetTexture("TitleBG.png");

	TestFont = CreateComponent<UIFontRender>(2);
	TestFont->SetFont("DMC5Font");
	TestFont->SetFontFlag(FW1_CENTER);
	TestFont->SetScale(36);
	TestFont->SetColor(float4::WHITE);
	TestFont->SetText("STATUS");
}

void TitleBG::Update(float _DeltaTime)
{

}

