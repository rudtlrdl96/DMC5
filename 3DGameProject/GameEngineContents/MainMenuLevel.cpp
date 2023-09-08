#include "PrecompileHeader.h"
#include "MainMenuLevel.h"
#include "MainMenuBG.h"
#include "Menu_SelectScreen.h"
#include "Menu_MissionFont.h"
#include "Menu_NeroInfo.h"
#include "Menu_VirgilInfo.h"
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

	std::vector<GameEngineFile> File = NewDir.GetAllFile({ ".Png", });


	for (size_t i = 0; i < File.size(); i++)
	{
		GameEngineTexture::Load(File[i].GetFullPath());
	}
	GetMainCamera()->SetProjectionType(CameraType::Orthogonal);
	GetMainCamera()->GetTransform()->SetLocalPosition({ 0, 0, -1000.0f });
	GetMainCamera()->GetCamTarget()->DepthSettingOff();

	std::shared_ptr<MainMenuBG> MainMenuBGPtr = CreateActor<MainMenuBG>();
	FEffect = GetLastTarget()->CreateEffect<FadeEffect>();
	std::shared_ptr<Menu_SelectScreen> Menu_SelectScreenPtr = CreateActor<Menu_SelectScreen>();
	std::shared_ptr<Menu_MissionFont> Menu_MissionFontPtr = CreateActor<Menu_MissionFont>();
	Menu_MissionFontPtr->GetTransform()->SetLocalPosition({ -680.0f,330.0f,0.0f });

	//���� �÷��̾ �׷ζ��
	//std::shared_ptr<Menu_NeroInfo> Menu_NeroInfoPtr = CreateActor<Menu_NeroInfo>();

	//���� �÷��̾ �����̶��,
	std::shared_ptr<Menu_VirgilInfo> Menu_VirgilInfoPtr = CreateActor<Menu_VirgilInfo>();
	
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
	FEffect->FadeOut();
}

void MainMenuLevel::LevelChangeEnd()
{
}
