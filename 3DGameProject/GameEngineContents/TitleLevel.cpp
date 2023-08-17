#include "PrecompileHeader.h"
#include "TitleLevel.h"
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
	GetMainCamera()->SetSortType(1, SortType::ZSort);

	FEffect = GetLastTarget()->CreateEffect<FadeEffect>();
	//ø¢≈Õ
	std::shared_ptr<TitleBG> TitleBGPtr = CreateActor<TitleBG>();

}

void TitleLevel::Update(float _DeltaTime)
{

}

void TitleLevel::LevelChangeStart()
{
	FEffect->FadeOut();
}

void TitleLevel::LevelChangeEnd()
{
}
