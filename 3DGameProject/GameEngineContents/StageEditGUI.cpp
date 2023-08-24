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
    /////////////////Vector Resize/////////////////////
	if (nullptr == Level)
		return;
    PreChar.resize(StartPrecharSize);
    EditData.resize(StartPrecharSize);
    Editbool.resize(StartPrecharSize);
    /////////////////StageEditButton/////////////////////
    if (ImGui::Button("StageEdit"))
    {
        BoxSwitch = !BoxSwitch;
    }
    if (true == BoxSwitch)
    {
        /////////////////Add Stage/////////////////////
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
            /////////////////Edit,Delete Button/////////////////////
            if (current == i && ImGui::Button("Delete"))
            {
                //delete
            }
            ImGui::SameLine();
            if (current == i && ImGui::Button("Edit"))
            {
                Editbool[i] = !Editbool[i];
                EditData[i] = i;
            }
            if (Editbool[i] == true && current== EditData[i])
            {
                ImGui::SeparatorText("StageDataEdit");
                static char Text[64] = ""; ImGui::InputText("FBX Name", Text, 64);
                ImGui::SameLine();
                if (ImGui::Button("Load"))
                {
                    char* Test = Text;
                }
            }
            if (current != EditData[i])
            {
                Editbool[i] = false;
            }
        }
    }
}
    