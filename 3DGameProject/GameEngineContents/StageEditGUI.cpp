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
        static int current = 0;
        ImGui::ListBox("StageDataBox", &current, StageNumber.data(), static_cast<int>(StageNumber.size()), EditBoxSize);
        ImGui::SameLine();
        for (int i = 0; i < StageNumber.size(); ++i)
        {
            if (current == i && ImGui::Button("Delete"))
            {
                //delete
            }
            ImGui::SameLine();
            if (current == i && ImGui::Button("Edit"))
            {
                EditSwitch = !EditSwitch;
                /*ImGui::DragScalar("drag s8", ImGuiDataType_S8, &s8_v, drag_speed, drag_clamp ? &s8_zero : NULL, drag_clamp ? &s8_fifty : NULL);*/
                /*P_StageData.MapDatas[i].MeshFileName*/
            }
            if (EditSwitch == true)
            {
                ImGui::SeparatorText("StageDataEdit");
            }
        }
       
    }
}
    