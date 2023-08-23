#include "PrecompileHeader.h"
#include "IntroLevel.h"

#include "BackGround.h"
#include "Logo.h"
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
	std::shared_ptr<BackGround> BGPtr = CreateActor<BackGround>();
	std::shared_ptr<Logo> LogoPtr = CreateActor<Logo>();

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
