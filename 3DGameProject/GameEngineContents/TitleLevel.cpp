#include "PrecompileHeader.h"
#include "TitleLevel.h"
#include "Title_StartScreen.h"
#include "TitleBG.h"
#include "FadeEffect.h"
#include <GameEngineCore/GameEngineFont.h>
#include <GameEngineCore/GameEngineTexture.h>
#include <GameEnginePlatform/GameEngineInput.h>
TitleLevel::TitleLevel()
{
}

TitleLevel::~TitleLevel()
{

}

void TitleLevel::Start()
{

	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Texture");
	NewDir.Move("UI");
	NewDir.Move("TitleTexture");

	std::vector<GameEngineFile> File = NewDir.GetAllFile({ ".Png", });


	for (size_t i = 0; i < File.size(); i++)
	{
		GameEngineTexture::Load(File[i].GetFullPath());
	}
	GetMainCamera()->SetProjectionType(CameraType::Orthogonal);
	GetMainCamera()->GetTransform()->SetLocalPosition({ 0, 0, -1000.0f });
	GetMainCamera()->GetCamTarget()->DepthSettingOff();

	FEffect = GetLastTarget()->CreateEffect<FadeEffect>();
	//ø¢≈Õ
	std::shared_ptr<TitleBG> TitleBGPtr = CreateActor<TitleBG>();
	TitleBGPtr->GetTransform()->SetLocalPosition({ 0.0f,0.0f,0.0f });
	std::shared_ptr<Title_StartScreen> Title_StartScreenPtr = CreateActor<Title_StartScreen>();
	Title_StartScreenPtr->GetTransform()->SetLocalPosition({ 0.0f,0.0f,0.0f });
}

void TitleLevel::Update(float _DeltaTime)
{
	if (GameEngineInput::IsDown("UIDEBUGMODE"))
	{
		IsDebugSwitch();
	}
	BaseLevel::Update(_DeltaTime);
}

void TitleLevel::LevelChangeStart()
{
	FEffect->FadeIn();
}

void TitleLevel::LevelChangeEnd()
{
}
