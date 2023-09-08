#include "PrecompileHeader.h"
#include "StageEditGUI.h"
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineBase/GameEngineFile.h>
#include "StageBaseLevel.h"
#include "FieldMap.h"
#include "MapCollisionMesh.h"

StageEditGUI::StageEditGUI()
{
}

StageEditGUI::~StageEditGUI()
{
}

void StageEditGUI::OnGUI(std::shared_ptr<GameEngineLevel> _Level, float _DeltaTime)
{
	if (Parent == nullptr)
	{
		Parent = _Level->DynamicThis<StageBaseLevel>().get();
	}

    StageListBox(_Level);

	if (IsCreateStage == true)
	{
		IsCreateStage = false;
		return;
	}

	if (AllData.empty())
	{
		return;
	}

	InputStageInfo(_Level);
}

void StageEditGUI::SaveStageData()
{
	GameEngineSerializer SaveSerializer = GameEngineSerializer();
	StageData::WriteAllStageData(SaveSerializer, AllData);

	GameEnginePath filepath;
	filepath.SetPath(GetSaveFilePath());

	if (filepath.GetFileName() == "")
	{
		return;
	}

	GameEngineFile file = GameEngineFile(std::string_view(filepath.GetFullPath()));
	file.SaveBin(SaveSerializer);
}

void StageEditGUI::LoadStageData(std::shared_ptr<GameEngineLevel> _Level)
{
	GameEngineSerializer LoadSerializer = GameEngineSerializer();

	std::filesystem::path Temp = GetOpenFilePath();
	if (Temp == "")
	{
		return;
	}
	GameEngineFile File(Temp);
	File.LoadBin(LoadSerializer);

	StageData::ReadAllStageData(LoadSerializer, AllData);

	CreateStage(AllData[0]);
}

void StageEditGUI::MapBundleCombo()
{
	if (AllData[Stage_current].MapDatas.empty())
	{
		return;
	}

	std::string temp = std::to_string(MapBundle_current);
	const char* combo_preview_value = temp.c_str();
	if (ImGui::BeginCombo("MapBundle", combo_preview_value, 0))
	{
		for (int n = 0; n < AllData[Stage_current].MapDatas.size(); n++)
		{
			const bool is_selected = (MapBundle_current == n);
			if (ImGui::Selectable(std::to_string(n).c_str(), is_selected))
				MapBundle_current = n;

			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	FieldMapAddButton();
}

void StageEditGUI::FieldMapAddButton()
{
	if (AllData[Stage_current].MapDatas.empty())
	{
		return;
	}

	if (ImGui::Button("AddMap"))
	{
		std::filesystem::path filepath = GetOpenFilePath();

		if (!filepath.empty())
		{
			pushback_Data(AllData[Stage_current].MapDatas.find(MapBundle_current)->second);
			auto TempIter = AllData[Stage_current].MapDatas.find(MapBundle_current)->second.end();
			TempIter--;
			TempIter->MeshFileName = filepath.filename().string();
			Parent->CreateStageFieldMap(AllData[Stage_current].MapDatas);
		}
	}
	FieldMapCombo();
}

void StageEditGUI::FieldMapCombo()
{
	if (AllData[Stage_current].MapDatas.find(MapBundle_current)->second.empty())
	{
		return;
	}
	
	const char* combo_preview_value = AllData[Stage_current].MapDatas.find(MapBundle_current)->second[FieldMap_current].MeshFileName.c_str();
	if (ImGui::BeginCombo("FieldMap", combo_preview_value, 0))
	{
		for (int n = 0; n < AllData[Stage_current].MapDatas.find(MapBundle_current)->second.size(); n++)
		{
			const bool is_selected = (FieldMap_current == n);
			if (ImGui::Selectable(AllData[Stage_current].MapDatas.find(MapBundle_current)->second[n].MeshFileName.c_str(), is_selected))
				FieldMap_current = n;

			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

void StageEditGUI::FieldMapTransformEditUI(std::shared_ptr<GameEngineObject> _Obj)
{
	ImGui::SeparatorText("FieldMapTransform");
	ShowTransformInfo(_Obj);
	if (ImGui::Button("InputTransform"))
	{
		GameEngineTransform* Transform = _Obj->GetTransform();
		AllData[Stage_current].MapDatas.find(MapBundle_current)->second[FieldMap_current].FieldMapPosition = Transform->GetLocalPosition();
		AllData[Stage_current].MapDatas.find(MapBundle_current)->second[FieldMap_current].FieldMapScale = Transform->GetLocalScale();
		AllData[Stage_current].MapDatas.find(MapBundle_current)->second[FieldMap_current].FieldMapRotation = Transform->GetLocalRotation();

		ImGui::SameLine();
	}
}

//void StageEditGUI::FieldMapColList()
//{
//	ImGui::SeparatorText("FieldMapColList");
//	ImGui::BeginChild("FieldMapColList", ImVec2(150, 0), true);
//	ImGui::EndChild();
//}

void StageEditGUI::ShowTransformInfo(std::shared_ptr<GameEngineObject> _Obj)
{
	if (_Obj == nullptr)
	{
		return;
	}

	GameEngineTransform* Transform = _Obj->GetTransform();
	float4 Position = Transform->GetLocalPosition();
	float4 Rotation = Transform->GetLocalRotation();
	float4 Scale = Transform->GetLocalScale();

	// TransData ǥ�� �� TransformUpdate�� ���� 0��ġ�̵� ����

	ImGui::DragFloat4("Position", Position.Arr1D);
	ImGui::DragFloat4("Rotation", Rotation.Arr1D);
	ImGui::DragFloat4("Scale", Scale.Arr1D);

	Transform->SetLocalPosition(Position);
	Transform->SetLocalRotation(Rotation);
	Transform->SetLocalScale(Scale);
}

void StageEditGUI::StageListBox(std::shared_ptr<GameEngineLevel> _Level)
{
    ImGui::InputText("input StageName", StageNameInputSpace, IM_ARRAYSIZE(StageNameInputSpace));


	

    if (ImGui::Button("AddStg"))
    {
        if (std::string(StageNameInputSpace) != "")
        {
            pushback_Data(AllData);
            auto TempIter = AllData.end();
            TempIter--;
            TempIter->StageName = StageNameInputSpace;
        }
		return;
	}
    ImGui::SameLine();
    if (ImGui::Button("DelStg"))
    {
        erase_Data(AllData, Stage_current);
		if (!AllData.empty())
		{
			if (AllData[Stage_current].StageName != Parent->StageName)
			{
				Parent->StageName = AllData[Stage_current].StageName;
				CreateStage(AllData[Stage_current]);
			}
		}
		return;
	}
	ImGui::SameLine();
	if (ImGui::Button("LoadStg"))
	{
		LoadStageData(_Level);
		return;
	}
	ImGui::SameLine();
	if (ImGui::Button("SaveStg"))
	{
		SaveStageData();
		return;
	}

    ImGui::BeginChild("StageList", ImVec2(150, 0), true);
    for (size_t i = 0; i < AllData.size(); i++)
    {
        std::string StageLabel = AllData[i].StageName;
		if (ImGui::Selectable(StageLabel.c_str(), Stage_current == i))
		{
			Stage_current = i;
			Parent->ClearStage();
			Parent->StageName = AllData[Stage_current].StageName;
			if (!AllData[Stage_current].MapDatas.empty())
			{
				CreateStage(AllData[Stage_current]);
			}
		}
    }
    ImGui::EndChild();
    ImGui::SameLine();
}

void StageEditGUI::InputStageInfo(std::shared_ptr<GameEngineLevel> _Level)
{
	ImGui::SameLine();
	ImGui::BeginChild("InputStageInfo", ImVec2(300, 0), true);
	if (ImGui::Button("SetSkyBox"))
	{
		ImGui::SeparatorText("StageInfo");
		std::filesystem::path filepath = GetOpenFilePath();
		if (!filepath.empty())
		{
			Parent->EraseSkyBox();
			AllData[Stage_current].SkyboxFileName = filepath.filename().string();
			Parent->CreateSkyBox(AllData[Stage_current].SkyboxFileName);
		}
	}

	ImGui::SameLine();
	ImGui::Text(AllData[Stage_current].SkyboxFileName.c_str());

	if (ImGui::Button("SetGroundMesh"))
	{
		std::filesystem::path filepath = GetOpenFilePath();
		if (!filepath.empty())
		{
			Parent->EraseGroundCol();
			AllData[Stage_current].GroundMeshFileName = filepath.filename().string();
			Parent->CreateGroundCol(AllData[Stage_current].GroundMeshFileName);
		}
	}

	ImGui::SameLine();

	ImGui::Text(AllData[Stage_current].GroundMeshFileName.c_str());

	if (!AllData[Stage_current].GroundMeshFileName.empty())
	{
		static bool check = true;
		ImGui::Checkbox("Ground On/Off", &check);
		if (Parent->AcGroundCol != nullptr)
		{
			if (check && !Parent->AcGroundCol->RenderIsUpdate())
			{
				Parent->AcGroundCol->RenderOn();
			}
			else if (!check && Parent->AcGroundCol->RenderIsUpdate())
			{
				Parent->AcGroundCol->RenderOff();
			}
		}
	}

	if (ImGui::Button("SetWallMesh"))
	{
		std::filesystem::path filepath = GetOpenFilePath();
		if (!filepath.empty())
		{
			Parent->EraseWallCol();
			AllData[Stage_current].WallMeshFileName = filepath.filename().string();
			Parent->CreateWallCol(AllData[Stage_current].WallMeshFileName);
		}
	}

	ImGui::SameLine();

	ImGui::Text(AllData[Stage_current].WallMeshFileName.c_str());

	if (!AllData[Stage_current].WallMeshFileName.empty())
	{
		static bool check = true;
		ImGui::Checkbox("Wall On/Off", &check);
		if (Parent->AcWallCol != nullptr)
		{
			if (check && !Parent->AcWallCol->RenderIsUpdate())
			{
				Parent->AcWallCol->RenderOn();
			}
			else if (!check && Parent->AcWallCol->RenderIsUpdate())
			{
				Parent->AcWallCol->RenderOff();
			}
		}
	}

	if (ImGui::Button("AddMapBundle"))
	{
		AllData[Stage_current].MapDatas.insert(std::make_pair(MapBundleIndex++, std::vector<FieldMapData>()));
	}
	
	ImGui::SameLine();
	
	if (ImGui::Button("DelMapBundle"))
	{

		AllData[Stage_current].MapDatas.erase(MapBundleIndex--);
	}

	MapBundleCombo();

	//if (!AllData[Stage_current].MapDatas.empty())
	//{
	//	FieldMapTransformEditUI(Parent->AcFieldMaps.find(MapBundle_current)->second[FieldMap_current]);
	//}

	ImGui::EndChild();
}

void StageEditGUI::CreateStage(StageData _Data)
{
	Parent->ClearStage();
	Parent->CreateStage(_Data);
	IsCreateStage = true;
}

std::string StageEditGUI::GetOpenFilePath()
{
	OPENFILENAME ofn;       // common dialog box structure
	wchar_t szFile[260];       // buffer for file name
	wchar_t PrevPath[260];
	GetCurrentDirectory(260, PrevPath);

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GameEngineWindow::GetHWnd();
	ofn.lpstrFile = szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = L'\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 

	if (GetOpenFileName(&ofn) == TRUE)
	{
		SetCurrentDirectory(PrevPath);
		return GameEngineString::UniCodeToAnsi(ofn.lpstrFile);
	}

	SetCurrentDirectory(PrevPath);
	return "";
}

std::string StageEditGUI::GetSaveFilePath()
{
	OPENFILENAME ofn;       // common dialog box structure
	wchar_t szFile[260];       // buffer for file name
	wchar_t PrevPath[260];
	GetCurrentDirectory(260, PrevPath);

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GameEngineWindow::GetHWnd();
	ofn.lpstrFile = szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = L'\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 

	if (GetSaveFileName(&ofn) == TRUE)
	{
		SetCurrentDirectory(PrevPath);
		return GameEngineString::UniCodeToAnsi(ofn.lpstrFile);
	}

	SetCurrentDirectory(PrevPath);
	return "";
}