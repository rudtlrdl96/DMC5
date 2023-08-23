#include "PrecompileHeader.h"
#include "StageEditGUI.h"
#include <GameEngineCore/GameEngineLevel.h>

using namespace std;

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
    PreChar.resize(StartPrecharSize);
    if (ImGui::Button("StageEdit"))
    {
        BoxSwitch = !BoxSwitch;
    }
    if (true == BoxSwitch)
    {
        if (ImGui::Button("AddStageData") && NumberIndex < StartPrecharSize - 1)
        {
           NumberIndex++;
           PreChar[NumberIndex] = "stage" + to_string(NumberIndex);
           StageNumber.push_back(PreChar[NumberIndex].c_str());
        }
        current = 0;
        ImGui::ListBox("StageDataBox", &current, StageNumber.data(), static_cast<int>(StageNumber.size()), EditBoxSize);
        ImGui::SameLine();
    }
}
    