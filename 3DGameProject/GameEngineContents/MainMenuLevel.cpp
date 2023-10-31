#include "PrecompileHeader.h"
#include "MainMenuLevel.h"
#include "MainMenuBG.h"
#include "Menu_SelectScreen.h"
#include "Menu_MissionFont.h"
#include "Menu_NeroInfo.h"
#include "Menu_VirgilInfo.h"
#include "Char_ChoiceUI.h"
#include "FadeEffect.h"
#include <GameEngineCore/GameEngineFont.h>
#include <GameEngineCore/GameEngineTexture.h>
#include <GameEnginePlatform/GameEngineInput.h>
MainMenuLevel::MainMenuLevel()
{
}

MainMenuLevel::~MainMenuLevel()
{

}

void MainMenuLevel::Start()
{

	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Texture");
	NewDir.Move("UI");
	NewDir.Move("MainMenuTexture");
	NewDir.Move("MenuTexture");
	if (nullptr == GameEngineTexture::Find("extext.png"))
	{
		std::vector<GameEngineFile> File = NewDir.GetAllFile({ ".Png", });
		for (size_t i = 0; i < File.size(); i++)
		{
			GameEngineTexture::Load(File[i].GetFullPath());
		}
	}

	std::shared_ptr<MainMenuBG> MainMenuBGPtr = CreateActor<MainMenuBG>();
	std::shared_ptr<Menu_SelectScreen> Menu_SelectScreenPtr = CreateActor<Menu_SelectScreen>();
	std::shared_ptr<Menu_MissionFont> Menu_MissionFontPtr = CreateActor<Menu_MissionFont>();
	Menu_VirgilInfoPtr = CreateActor<Menu_VirgilInfo>();
	Menu_VirgilInfoPtr->Off();
	Menu_NeroInfoPtr = CreateActor<Menu_NeroInfo>();
	Menu_NeroInfoPtr->Off();
	GetMainCamera()->SetProjectionType(CameraType::Orthogonal);
	GetMainCamera()->GetTransform()->SetLocalPosition({ 0, 0, -1000.0f });
	GetMainCamera()->GetCamTarget()->DepthSettingOff();

	FEffect = GetLastTarget()->CreateEffect<FadeEffect>();
	FEffect->FadeIn();

	//만약 플레이어가 네로라면
	//

	//만약 플레이어가 버질이라면,
	
}

void MainMenuLevel::Update(float _DeltaTime)
{
	if (GameEngineInput::IsDown("UIDEBUGMODE"))
	{
		IsDebugSwitch();
	}
	BaseLevel::Update(_DeltaTime);
}

void MainMenuLevel::LevelChangeStart()
{

	if (Char_ChoiceUI::GetPlayerType() == ChoicePlayerType::NERO)
	{
		Menu_NeroInfoPtr->On();
	}
	else if (Char_ChoiceUI::GetPlayerType() == ChoicePlayerType::VERGIL)
	{
		Menu_VirgilInfoPtr->On();
	}
	FEffect->FadeIn();

}

void MainMenuLevel::LevelChangeEnd()
{
}
