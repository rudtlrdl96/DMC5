#include "PrecompileHeader.h"
#include "IntroLevel.h"

#include "Intro_BackGround.h"
#include "Intro_Logo.h"
#include <GameEngineCore/GameEngineFont.h>
#include <GameEnginePlatform/GameEngineSound.h>
IntroLevel::IntroLevel()
{
}

IntroLevel::~IntroLevel()
{
}

void IntroLevel::Start()
{
	GameEngineFont::Load("DMC5Font");
	GameEngineFont::Load("���� ���");
	GameEngineFont::Load("Tahoma Bold");
	GameEngineFont::Load("�����ٸ���� ���ѱ�");
	GameEngineFont::Load("Arial Bold");

	GetMainCamera()->SetProjectionType(CameraType::Orthogonal);
	GetMainCamera()->GetTransform()->SetLocalPosition({ 0, 0, -1000.0f });



}

void IntroLevel::Update(float _DeltaTime)
{
	Time += _DeltaTime;
	if (Time > 11.0f)
	{
		GameEngineCore::ChangeLevel("TitleLevel");
	}
	BaseLevel::Update(_DeltaTime);

}

void IntroLevel::LevelChangeStart()
{
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

	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Texture");
	NewDir.Move("UI");
	NewDir.Move("IntroTexture");
	if (nullptr == GameEngineTexture::Find("BG.png"))
	{
		std::vector<GameEngineFile> File = NewDir.GetAllFile({ ".Png", });
		for (size_t i = 0; i < File.size(); i++)
		{
			GameEngineTexture::Load(File[i].GetFullPath());
		}
	}

	std::shared_ptr<Intro_BackGround> BGPtr = CreateActor<Intro_BackGround>();
	std::shared_ptr<Intro_Logo> IntroLogoPtr = CreateActor<Intro_Logo>();

}

void IntroLevel::LevelChangeEnd()
{
}
