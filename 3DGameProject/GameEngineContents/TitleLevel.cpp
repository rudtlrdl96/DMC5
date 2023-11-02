#include "PrecompileHeader.h"
#include "TitleLevel.h"
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineFont.h>
#include <GameEngineCore/GameEngineTexture.h>
#include "Title_StartScreen.h"
#include "TitleBG.h"
#include "FadeEffect.h"
#include "BGMPlayer.h"
#include "Title_SelectScreen.h"
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
	if (nullptr == GameEngineTexture::Find("NullTexture.png"))
	{
		std::vector<GameEngineFile> File = NewDir.GetAllFile({ ".Png", });
		for (size_t i = 0; i < File.size(); i++)
		{
			GameEngineTexture::Load(File[i].GetFullPath());
		}
	}
	GameEngineDirectory SoundDir;
	SoundDir.MoveParentToDirectory("ContentResources");
	SoundDir.Move("ContentResources");
	SoundDir.Move("Sound");
	SoundDir.Move("UISound");
	if (nullptr == GameEngineSound::Find("StartButtonSelect.wav"))
	{
		std::vector<GameEngineFile> SoundFile = SoundDir.GetAllFile({ ".wav", });
		for (size_t i = 0; i < SoundFile.size(); i++)
		{
			GameEngineSound::Load(SoundFile[i].GetFullPath());
		}
	}
	GetMainCamera()->SetProjectionType(CameraType::Orthogonal);
	GetMainCamera()->GetTransform()->SetLocalPosition({ 0, 0, -1000.0f });
	GetMainCamera()->GetCamTarget()->DepthSettingOff();

	FEffect = GetLastTarget()->CreateEffect<FadeEffect>();

	std::shared_ptr<TitleBG> TitleBGPtr = CreateActor<TitleBG>();
	TitleBGPtr->GetTransform()->SetLocalPosition({ 0.0f,0.0f,0.0f });
	std::shared_ptr<Title_SelectScreen> Title_StartScreenPtr = CreateActor<Title_SelectScreen>();
	Title_StartScreenPtr->GetTransform()->SetLocalPosition({ 0.0f,0.0f,0.0f });
	//¿¢ÅÍ

}

void TitleLevel::Update(float _DeltaTime)
{
	if (GameEngineInput::IsDown("UIDEBUGMODE"))
	{
		IsDebugSwitch();
	}
	if (GameEngineInput::IsDown("UI_ESC"))
	{
		GameEngineWindow::AppOff();
	}
	
	BaseLevel::Update(_DeltaTime);
}

void TitleLevel::LevelChangeStart()
{


	BGMPlayer::SetTitleBGM();
	FEffect->FadeIn();
}

void TitleLevel::LevelChangeEnd()
{
	BGMPlayer::SetBGMFadeOut();
}
