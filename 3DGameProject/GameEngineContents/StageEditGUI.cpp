#include "PrecompileHeader.h"
#include "StageEditGUI.h"
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineActor.h>
using namespace std;

#include "FieldMap.h"

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
    P_StageData.resize(StartPrecharSize);
    //for (auto i = 0; i < P_StageData.size(); ++i)
    //{
    //    StageData Datas;
    //    P_StageData.push_back(Datas);
    //}
    ///////////////////StageEditButton/////////////////////
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
            if (Editbool[i] == true && current== EditData[i]) //여기를 구분지을것이 필요함
            {
                ImGui::SeparatorText("StageDataEdit");
                static char Text[64] = ""; ImGui::InputText("Mesh1 FBX Name", Text, 64);
                ImGui::SameLine();
                if (ImGui::Button("Load"))
                {
                    char* FBXName = Text;
                    P_StageData[current].MapDatas[current].MeshFileName = FBXName;
                    /*P_Field.CreateFieldMap(Level.get(), FBXName);*/
                    /*std::vector<StageData> AllDatas;*/
                }
            }
            if (current != EditData[i])
            {
                Editbool[i] = false;
            }
        }
    }
}
    