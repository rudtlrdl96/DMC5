#include "PrecompileHeader.h"
#include "StageEditGUI.h"
#include <GameEngineCore/GameEngineLevel.h>

StageEditGUI::StageEditGUI()
{
}

StageEditGUI::~StageEditGUI()
{
}

void StageEditGUI::OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime)
{
	if (nullptr == Level)
		return;
	if (true == ImGui::Button("Button"))
	{
		ImGui::TreeNode("Stage List");
		const char* items[] = { "Stage1", "Stage2", "Stage3", "Stage4" };
		static int item_current_idx = 0;
	}
}


