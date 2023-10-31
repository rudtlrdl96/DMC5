#include "PrecompileHeader.h"
#include "Char_ChoiceLevel.h"
#include "C_ChoiceBG.h"
#include "Char_ChoiceUI.h"
#include "BGMPlayer.h"
#include "FadeEffect.h"
#include <GameEngineCore/GameEngineFont.h>
#include <GameEngineCore/GameEngineTexture.h>
#include <GameEnginePlatform/GameEngineInput.h>
Char_ChoiceLevel::Char_ChoiceLevel()
{
}

Char_ChoiceLevel::~Char_ChoiceLevel()
{

}

void Char_ChoiceLevel::Start()
{

	GetMainCamera()->SetProjectionType(CameraType::Orthogonal);
	GetMainCamera()->GetTransform()->SetLocalPosition({ 0, 0, -1000.0f });
	FEffect = GetLastTarget()->CreateEffect<FadeEffect>();



}

void Char_ChoiceLevel::Update(float _DeltaTime)
{
	BaseLevel::Update(_DeltaTime);
}

void Char_ChoiceLevel::LevelChangeStart()
{
	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Texture");
	NewDir.Move("UI");
	NewDir.Move("CharChoiseTexture");
	if (nullptr == GameEngineTexture::Find("Char_Bar.png"))
	{
		std::vector<GameEngineFile> File = NewDir.GetAllFile({ ".Png", });
		for (size_t i = 0; i < File.size(); i++)
		{
			GameEngineTexture::Load(File[i].GetFullPath());
		}
	}



	std::shared_ptr<C_ChoiceBG> C_ChoiceBGPtr = CreateActor<C_ChoiceBG>();
	std::shared_ptr<Char_ChoiceUI> Char_ChoiceUIPtr = CreateActor<Char_ChoiceUI>();
	BGMPlayer::SetMissionBGM();
	FEffect->FadeIn();
}

void Char_ChoiceLevel::LevelChangeEnd()
{
}
