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
	GameEngineGUI::GUIWindowCreate<StageEditGUI>("StageEditGUI");
}

void StageEditLevel::Update(float _DeltaTime)
{
	
}

