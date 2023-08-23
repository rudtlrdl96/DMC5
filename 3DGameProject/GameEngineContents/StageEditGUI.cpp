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
    if (ImGui::Button("StageEdit"))
    {
        BoxSwitch = !BoxSwitch;
    }
    if (true == BoxSwitch)
    {
        if (ImGui::Button("AddStageData"))
        {
           NumberIndex++;
           PreChar = StageNameSet(NumberIndex);
           StageNumber.push_back(PreChar);
        }
        ImGui::ListBox("StageDataBox", &current, StageNumber.data(), static_cast<int>(StageNumber.size()), EditBoxSize);
        current = 1;
        ImGui::SameLine();
    }
}

const char* StageEditGUI::StageNameSet(int _NumberIndex)
{
    switch (_NumberIndex)
    {
    case 1:
        return "Stage1";  break;
    case 2:
        return "Stage2"; break;
    case 3:
        return "Stage3"; break;
    case 4:
        return "Stage4"; break;
    case 5:
        return "Stage5"; break;
    case 6:
        return "Stage6"; break;
    case 7:
        return "Stage7"; break;
    default:
        break;
    }
}


