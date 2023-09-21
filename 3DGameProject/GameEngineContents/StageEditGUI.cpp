#include "PrecompileHeader.h"
#include "StageEditGUI.h"
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineBase/GameEngineFile.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include "StageEditLevel.h"
#include "FieldMap.h"
#include "MapCollisionMesh.h"
#include "FreeCameraActor.h"
#include "FieldMapObjEditGUI.h"
#include "SkyBox.h"

StageEditGUI* StageEditGUI::MainEditor = nullptr;

StageEditGUI::StageEditGUI()
{
	MainEditor = this;
}

StageEditGUI::~StageEditGUI()
{
}

void StageEditGUI::OnGUI(std::shared_ptr<GameEngineLevel> _Level, float _DeltaTime)
{
	if (Parent == nullptr)
	{
		Parent = _Level->DynamicThis<StageEditLevel>().get();
	}

	static bool FreeCamCheck;
	ImGui::Checkbox("FreeCam On/Off", &FreeCamCheck);
	if (FreeCamCheck && !Parent->FreeCam->IsUpdate())
	{
		Parent->FreeCam->On();
	}
	else if(!FreeCamCheck && Parent->FreeCam->IsUpdate())
	{
		Parent->FreeCam->Off();
	}

    StageCombo(_Level);

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

	if (!AllData.empty())
	{
		CreateStage(AllData[0]);
	}
}

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

	// TransData 표시 및 TransformUpdate를 위한 0위치이동 실행

	ImGui::DragFloat4("Position", Position.Arr1D, 50.f);
	ImGui::DragFloat4("Rotation", Rotation.Arr1D);
	ImGui::DragFloat4("Scale", Scale.Arr1D, 50.f);

	Transform->SetLocalPosition(Position);
	Transform->SetLocalRotation(Rotation);
	Transform->SetLocalScale(Scale);
}

void StageEditGUI::StageCombo(std::shared_ptr<GameEngineLevel> _Level)
{
    ImGui::InputText("input StageName", StageNameInputSpace, IM_ARRAYSIZE(StageNameInputSpace));

    if (ImGui::Button("AddStg"))
    {
        if (std::string(StageNameInputSpace) != "")
        {
			AllData.emplace_back().StageName = StageNameInputSpace;
        }
		return;
	}

    ImGui::SameLine();
    
	if (ImGui::Button("DelStg"))
    {
		if (!AllData.empty())
		{
			AllData.pop_back();
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

	if (AllData.empty())
	{
		return;
	}

	const char* combo_preview_value = AllData[Stage_current].StageName.c_str();
	if (ImGui::BeginCombo("Stage", combo_preview_value, 0))
	{
		for (int n = 0; n < AllData.size(); n++)
		{
			const bool is_selected = (Stage_current == n);
			if (ImGui::Selectable(AllData[n].StageName.c_str(), is_selected))
				Stage_current = n;

			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

void StageEditGUI::InputStageInfo(std::shared_ptr<GameEngineLevel> _Level)
{
	ImGui::BeginChild("InputStageInfo", ImVec2(300, 0), true);
	InputSkyBox();
	InputMapCol();
	InputFieldMap();
	ImGui::EndChild();

	if (FieldObjEditor == nullptr)
	{
		FieldObjEditor = GameEngineGUI::GUIWindowCreate<FieldMapObjEditGUI>("FieldObjEditor");
	}

	if (!AllData[Stage_current].MapDatas.empty())
	{
		if (!FieldObjEditor->IsUpdate())
		{
			FieldObjEditor->On();
		}
	}
	else
	{
		FieldObjEditor->Off();
	}
}

void StageEditGUI::InputSkyBox()
{
	if (ImGui::Button("SetSkyBox"))
	{
		std::filesystem::path filepath = GetOpenFilePath();
		if (!filepath.empty())
		{
			AllData[Stage_current].SkyboxFileName = filepath.filename().string();
			CreateStage(AllData[Stage_current]);
		}
	}
	ImGui::SameLine();
	ImGui::Text(AllData[Stage_current].SkyboxFileName.c_str());

	if (!AllData[Stage_current].SkyboxFileName.empty())
	{
		static bool check = true;
		ImGui::Checkbox("Sky On/Off", &check);
		if (Parent->AcSkyBox.lock() != nullptr)
		{
			if (check && !Parent->AcSkyBox.lock()->IsUpdate())
			{
				Parent->AcSkyBox.lock()->On();
			}
			else if (!check && Parent->AcSkyBox.lock()->IsUpdate())
			{
				Parent->AcSkyBox.lock()->Off();
			}
		}
	}
}

void StageEditGUI::InputMapCol()
{
	if (ImGui::Button("SetGroundMesh"))
	{
		std::filesystem::path filepath = GetOpenFilePath();
		if (!filepath.empty())
		{
			AllData[Stage_current].GroundMeshFileName = filepath.filename().string();
			CreateStage(AllData[Stage_current]);
		}
	}
	ImGui::SameLine();
	ImGui::Text(AllData[Stage_current].GroundMeshFileName.c_str());

	if (!AllData[Stage_current].GroundMeshFileName.empty())
	{
		static bool check = true;
		ImGui::Checkbox("Ground On/Off", &check);
		if (Parent->AcGroundCol.lock() != nullptr)
		{
			if (check && !Parent->AcGroundCol.lock()->RenderIsUpdate())
			{
				Parent->AcGroundCol.lock()->RenderOn();
			}
			else if (!check && Parent->AcGroundCol.lock()->RenderIsUpdate())
			{
				Parent->AcGroundCol.lock()->RenderOff();
			}
		}
	}

	if (ImGui::Button("SetWallMesh"))
	{
		std::filesystem::path filepath = GetOpenFilePath();
		if (!filepath.empty())
		{
			AllData[Stage_current].WallMeshFileName = filepath.filename().string();
			CreateStage(AllData[Stage_current]);
		}
	}
	ImGui::SameLine();
	ImGui::Text(AllData[Stage_current].WallMeshFileName.c_str());

	if (!AllData[Stage_current].WallMeshFileName.empty())
	{
		static bool check = true;
		ImGui::Checkbox("Wall On/Off", &check);
		if (Parent->AcWallCol.lock() != nullptr)
		{
			if (check && !Parent->AcWallCol.lock()->RenderIsUpdate())
			{
				Parent->AcWallCol.lock()->RenderOn();
			}
			else if (!check && Parent->AcWallCol.lock()->RenderIsUpdate())
			{
				Parent->AcWallCol.lock()->RenderOff();
			}
		}
	}
}

void StageEditGUI::InputFieldMap()
{
	if (ImGui::Button("AddFieldMap"))
	{
		AllData[Stage_current].MapDatas.emplace_back();
		CreateStage(AllData[Stage_current]);
	}
	ImGui::SameLine();
	if (ImGui::Button("DelFieldMap"))
	{
		if (!AllData[Stage_current].MapDatas.empty())
		{
			AllData[Stage_current].MapDatas.pop_back();
			CreateStage(AllData[Stage_current]);
		}
	}

	if (AllData[Stage_current].MapDatas.empty())
	{
		return;
	}

	FieldMapCombo();

}

void StageEditGUI::FieldMapCombo()
{	
	ImGui::SeparatorText("FieldMapOption");

	std::string TempStr = std::string("FieldMap " + std::to_string(FieldMap_current));
	const char* combo_preview_value = TempStr.c_str();
	if (ImGui::BeginCombo("FieldMap", combo_preview_value, 0))
	{
		for (int n = 0; n < AllData[Stage_current].MapDatas.size(); n++)
		{
			const bool is_selected = (FieldMap_current == n);
			if (ImGui::Selectable(std::string("FieldMap " + std::to_string(n)).c_str(), is_selected))
				FieldMap_current = n;

			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	if (AllData[Stage_current].MapDatas.empty())
	{
		return;
	}

	InputMapParts();
	InputCullingCol();
	InputNode();
}

void StageEditGUI::InputMapParts()
{
	if (ImGui::Button("AddMapParts"))
	{
		std::filesystem::path filepath = GetOpenFilePath();
		if (!filepath.empty())
		{
			AllData[Stage_current].MapDatas[FieldMap_current].PartsMeshFileNames.emplace_back(filepath.filename().string());
			CreateStage(AllData[Stage_current]);
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("DelMapParts"))
	{
		if (!AllData[Stage_current].MapDatas[FieldMap_current].PartsMeshFileNames.empty())
		{
			AllData[Stage_current].MapDatas[FieldMap_current].PartsMeshFileNames.pop_back();
			CreateStage(AllData[Stage_current]);
		}
	}

	if (AllData[Stage_current].MapDatas[FieldMap_current].PartsMeshFileNames.empty())
	{
		return;
	}

	MapPartsCombo();
}

void StageEditGUI::MapPartsCombo()
{
	const char* combo_preview_value = AllData[Stage_current].MapDatas[FieldMap_current].PartsMeshFileNames[MapParts_current].c_str();
	if (ImGui::BeginCombo("MapRenders", combo_preview_value, 0))
	{
		for (int n = 0; n < AllData[Stage_current].MapDatas[FieldMap_current].PartsMeshFileNames.size(); n++)
		{
			const bool is_selected = (MapParts_current == n);
			if (ImGui::Selectable(AllData[Stage_current].MapDatas[FieldMap_current].PartsMeshFileNames[n].c_str(), is_selected))
				MapParts_current = n;

			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

void StageEditGUI::InputCullingCol()
{
	if (ImGui::Button("AddCullingCol"))
	{
		AllData[Stage_current].MapDatas[FieldMap_current].CullingColTransform.emplace_back();
		CreateStage(AllData[Stage_current]);
	}
	ImGui::SameLine();
	if (ImGui::Button("DelCullingCol"))
	{
		if (!AllData[Stage_current].MapDatas[FieldMap_current].CullingColTransform.empty())
		{
			AllData[Stage_current].MapDatas[FieldMap_current].CullingColTransform.pop_back();
			CreateStage(AllData[Stage_current]);
		}
	}

	if (AllData[Stage_current].MapDatas[FieldMap_current].CullingColTransform.empty())
	{
		return;
	}

	CullingColCombo();
}

void StageEditGUI::CullingColCombo()
{
	std::string TempStr = std::string("Col " + std::to_string(CullingCol_current));
	const char* combo_preview_value = TempStr.c_str();
	if (ImGui::BeginCombo("CullingCol", combo_preview_value, 0))
	{
		for (int n = 0; n < AllData[Stage_current].MapDatas[FieldMap_current].CullingColTransform.size(); n++)
		{
			const bool is_selected = (CullingCol_current == n);
			if (ImGui::Selectable(std::string("Col " + std::to_string(n)).c_str(), is_selected))
				CullingCol_current = n;

			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	ShowTransformInfo(Parent->AcFieldMaps[FieldMap_current].lock()->FieldMapCullingCol[CullingCol_current].lock());
	if (ImGui::Button("Input Transform"))
	{
		AllData[Stage_current].MapDatas[FieldMap_current].CullingColTransform[CullingCol_current].Pos 
			= Parent->AcFieldMaps[FieldMap_current].lock()->FieldMapCullingCol[CullingCol_current].lock()->GetTransform()->GetLocalPosition();
		AllData[Stage_current].MapDatas[FieldMap_current].CullingColTransform[CullingCol_current].Scale
			= Parent->AcFieldMaps[FieldMap_current].lock()->FieldMapCullingCol[CullingCol_current].lock()->GetTransform()->GetLocalScale();
		AllData[Stage_current].MapDatas[FieldMap_current].CullingColTransform[CullingCol_current].Rot
			= Parent->AcFieldMaps[FieldMap_current].lock()->FieldMapCullingCol[CullingCol_current].lock()->GetTransform()->GetLocalRotation();
	}
}

void StageEditGUI::InputNode()
{
	NodeCheckBox();
}

void StageEditGUI::NodeCheckBox()
{
	ImGui::Spacing();
	ImGui::Spacing();

	std::string Temp("FieldMap ");

	ImGui::Text("CullingMap Node Select");
	for (size_t i = 0; i < AllData[Stage_current].MapDatas.size(); i++)
	{
		ImGui::Checkbox((Temp + std::to_string(i)).c_str(), &NodeCheckBool[i]);
	
		if (i % 2 != 1)
		{
			ImGui::SameLine();
		}
	}

	ImGui::NewLine();

	if (ImGui::Button("CullingMap Node Input"))
	{
		AllData[Stage_current].MapDatas[FieldMap_current].NodeIndex.clear();
		AllData[Stage_current].MapDatas[FieldMap_current].NodeIndex.reserve(AllData[Stage_current].MapDatas.size());
		for (size_t i = 0; i < AllData[Stage_current].MapDatas[FieldMap_current].NodeIndex.capacity(); i++)
		{
			if (NodeCheckBool[i])
			{
				AllData[Stage_current].MapDatas[FieldMap_current].NodeIndex.emplace_back(static_cast<int>(i));
			}
		}
	}
}

void StageEditGUI::CreateStage(StageData _Data)
{
	Parent->ClearStage();
	Parent->CreateStage(_Data);
	for (auto& i : Parent->AcFieldMaps)
	{
		for (auto& j : i.lock()->FieldMapRenderer)
		{
			j.lock()->On();
		}
	}
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