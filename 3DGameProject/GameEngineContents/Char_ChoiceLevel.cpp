#include "PrecompileHeader.h"
#include "Char_ChoiceLevel.h"
#include "C_ChoiceBG.h"
#include "Char_ChoiceUI.h"

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
	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Texture");
	NewDir.Move("UI");
	NewDir.Move("CharChoiseTexture");

	std::vector<GameEngineFile> File = NewDir.GetAllFile({ ".Png", });


	for (size_t i = 0; i < File.size(); i++)
	{
		GameEngineTexture::Load(File[i].GetFullPath());
	}

	GetMainCamera()->SetProjectionType(CameraType::Orthogonal);
	GetMainCamera()->GetTransform()->SetLocalPosition({ 0, 0, -1000.0f });
	FEffect = GetLastTarget()->CreateEffect<FadeEffect>();

	std::shared_ptr<C_ChoiceBG> C_ChoiceBGPtr = CreateActor<C_ChoiceBG>();
	std::shared_ptr<Char_ChoiceUI> Char_ChoiceUIPtr = CreateActor<Char_ChoiceUI>();


}

void Char_ChoiceLevel::Update(float _DeltaTime)
{
	BaseLevel::Update(_DeltaTime);
}

void Char_ChoiceLevel::LevelChangeStart()
{
	FEffect->FadeIn();
}

void Char_ChoiceLevel::LevelChangeEnd()
{
}
