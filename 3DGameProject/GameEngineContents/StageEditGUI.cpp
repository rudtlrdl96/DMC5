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
    const char* StageName = "";

    switch (_NumberIndex)
    {
    case 1:
        StageName = "Stage1";
        return StageName;  break;
    case 2:
        StageName = "Stage2";
        return StageName; break;
    case 3:
        StageName = "Stage3";
        return StageName; break;
    case 4:
        StageName = "Stage4";
        return StageName; break;
    case 5:
        StageName = "Stage5";
        return StageName; break;
    case 6:
        StageName = "Stage6";
        return StageName; break;
    case 7:
        StageName = "Stage7";
        return StageName; break;
    default:
        break;
    }

    return StageName;
}


