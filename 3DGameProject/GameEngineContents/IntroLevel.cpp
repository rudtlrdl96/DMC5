#include "PrecompileHeader.h"
#include "IntroLevel.h"

#include "Intro_BackGround.h"
#include "Intro_Logo.h"
#include <GameEngineCore/GameEngineFont.h>
IntroLevel::IntroLevel()
{
}

IntroLevel::~IntroLevel()
{
}

void IntroLevel::Start()
{
	GameEngineFont::Load("DMC5Font");

	GetMainCamera()->SetProjectionType(CameraType::Orthogonal);
	GetMainCamera()->GetTransform()->SetLocalPosition({ 0, 0, -1000.0f });

	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Texture");
	NewDir.Move("UI");
	NewDir.Move("IntroTexture");

	std::vector<GameEngineFile> File = NewDir.GetAllFile({ ".Png", });


	for (size_t i = 0; i < File.size(); i++)
	{
		GameEngineTexture::Load(File[i].GetFullPath());
	}
	std::shared_ptr<Intro_BackGround> BGPtr = CreateActor<Intro_BackGround>();
	std::shared_ptr<Intro_Logo> IntroLogoPtr = CreateActor<Intro_Logo>();

}

void IntroLevel::Update(float _DeltaTime)
{
	Time += _DeltaTime;
	if (Time > 11.0f)
	{
		GameEngineCore::ChangeLevel("TitleLevel");
	}

}

void IntroLevel::LevelChangeStart()
{
}

void IntroLevel::LevelChangeEnd()
{
}
