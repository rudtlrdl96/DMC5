#include "PrecompileHeader.h"
#include "StageEditLevel.h"

#include <GameEngineCore/GameEngineGUI.h>

#include "NetworkManager.h"
#include "StageEditGUI.h"

StageEditLevel::StageEditLevel()
{
}

StageEditLevel::~StageEditLevel()
{
}

void StageEditLevel::Start()
{
	if (GetMainCamera() != nullptr)
	{
		GetMainCamera()->SetProjectionType(CameraType::Perspective);
	}
	GetMainCamera()->GetTransform()->SetLocalPosition({0,0,-500});
}

void StageEditLevel::Update(float _DeltaTime)
{
	//Test
	if (true == GameEngineInput::IsDown("SelectLevel_01"))
	{
		GameEngineCore::ChangeLevel("MainLevel");
		return;
	}
}

void StageEditLevel::LevelChangeStart()
{
	if (Edit == nullptr)
	{
		std::shared_ptr<GameEngineGUIWindow> EditGUI = GameEngineGUI::GUIWindowCreate<StageEditGUI>("StageEditGUI");
		Edit = EditGUI;
	}
	Edit->On();
}
void StageEditLevel::LevelChangeEnd()
{
	Edit->Off();
}

