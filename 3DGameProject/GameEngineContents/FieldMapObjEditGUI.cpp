#include "PrecompileHeader.h"
#include "FieldMapObjEditGUI.h"
#include "StageEditGUI.h"
#include "StageEditLevel.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>

FieldMapObjEditGUI::FieldMapObjEditGUI()
{

}

FieldMapObjEditGUI::~FieldMapObjEditGUI()
{

}

void FieldMapObjEditGUI::OnGUI(std::shared_ptr<GameEngineLevel> _Level, float _DeltaTime)
{
	if (Parent == nullptr)
	{
		Parent = StageEditGUI::MainEditor->Parent;
	}

	if (AllData_ptr == nullptr)
	{
		AllData_ptr = &StageEditGUI::MainEditor->AllData;
		Stage_current_ptr = &StageEditGUI::MainEditor->Stage_current;
		FieldMap_current_ptr = &StageEditGUI::MainEditor->FieldMap_current;
	}

	static std::vector<StageData>& AllData = *AllData_ptr;
	static size_t& Stage_current = *Stage_current_ptr;
	static size_t& FieldMap_current = *FieldMap_current_ptr;

	FieldMapCombo(AllData, Stage_current, FieldMap_current);
	ImGui::SeparatorText("Input Obj Option");
	InputObject(AllData, Stage_current, FieldMap_current);
}

void FieldMapObjEditGUI::ShowTransformInfo(std::shared_ptr<GameEngineObject> _Obj)
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

	ImGui::DragFloat4("Position", Position.Arr1D, 5.f);
	ImGui::DragFloat4("Rotation", Rotation.Arr1D);
	ImGui::DragFloat4("Scale", Scale.Arr1D);

	Transform->SetLocalPosition(Position);
	Transform->SetLocalRotation(Rotation);
	Transform->SetLocalScale(Scale);
}

void FieldMapObjEditGUI::CreateStage(StageData _Data)
{
	Parent->ClearStage();
	Parent->CreateStage(_Data);
	for (auto& i : Parent->AcFieldMaps)
	{
		for (auto& j : i->FieldMapRenderer)
		{
			j->On();
		}
	}
}

void FieldMapObjEditGUI::FieldMapCombo(std::vector<StageData>& _AllData, size_t& _Stage_current, size_t& _FieldMap_current)
{
	std::string TempStr = std::string("FieldMap " + std::to_string(_FieldMap_current));
	const char* combo_preview_value = TempStr.c_str();
	if (ImGui::BeginCombo("FieldMap", combo_preview_value, 0))
	{
		for (int n = 0; n < _AllData[_Stage_current].MapDatas.size(); n++)
		{
			const bool is_selected = (_FieldMap_current == n);
			if (ImGui::Selectable(std::string("FieldMap " + std::to_string(n)).c_str(), is_selected))
				_FieldMap_current = n;

			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

void FieldMapObjEditGUI::ObjTypeSelectCombo()
{
	//const char* items[] = { "Test0", "Test1" };
	const char* combo_preview_value = ObjType_items[ObjType_current];
	if (ImGui::BeginCombo("ObjType", combo_preview_value, 0))
	{
		for (int n = 0; n < IM_ARRAYSIZE(ObjType_items); n++)
		{
			const bool is_selected = (ObjType_current == n);
			if (ImGui::Selectable(ObjType_items[n], is_selected))
				ObjType_current = n;

			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

void FieldMapObjEditGUI::InputObject(std::vector<StageData>& _AllData, size_t& _Stage_current, size_t& _FieldMap_current)
{
	ObjTypeSelectCombo();
	
	if (ImGui::Button("AddObj"))
	{
		FieldMapObjData& tempData = _AllData[_Stage_current].MapDatas[_FieldMap_current].MapObjData.emplace_back();
		tempData.Type = static_cast<FieldMapObjType>(ObjType_current);
		CreateStage(_AllData[_Stage_current]);
	}
	ImGui::SameLine();
	if (ImGui::Button("DelObj"))
	{
		if (!_AllData[_Stage_current].MapDatas[_FieldMap_current].MapObjData.empty())
		{
			_AllData[_Stage_current].MapDatas[_FieldMap_current].MapObjData.pop_back();
		}
		CreateStage(_AllData[_Stage_current]);
	}

	if (_AllData[_Stage_current].MapDatas[_FieldMap_current].MapObjData.empty())
	{
		return;
	}

	ObjCombo(_AllData, _Stage_current, _FieldMap_current);
}

void FieldMapObjEditGUI::ObjCombo(std::vector<StageData>& _AllData, size_t& _Stage_current, size_t& _FieldMap_current)
{
	std::string tempStr = std::string("Obj" + std::to_string(Obj_current) + ":" + ObjType_items[static_cast<int>(_AllData[_Stage_current].MapDatas[_FieldMap_current].MapObjData[Obj_current].Type)]);
	const char* combo_preview_value = tempStr.c_str();
	if (ImGui::BeginCombo("MapObjs", combo_preview_value, 0))
	{
		for (int n = 0; n < _AllData[_Stage_current].MapDatas[_FieldMap_current].MapObjData.size(); n++)
		{
			const bool is_selected = (Obj_current == n);
			if (ImGui::Selectable(std::string("Obj" + std::to_string(n) + ":" + ObjType_items[static_cast<int>(_AllData[_Stage_current].MapDatas[_FieldMap_current].MapObjData[n].Type)]).c_str(), is_selected))
				Obj_current = n;

			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	ShowTransformInfo(Parent->AcFieldMaps[_FieldMap_current]->FieldMapObj[Obj_current]);

	if (ImGui::Button("Input ObjTransform"))
	{
		_AllData[_Stage_current].MapDatas[_FieldMap_current].MapObjData[Obj_current].ObjTransform.Pos =
			Parent->AcFieldMaps[_FieldMap_current]->FieldMapObj[Obj_current]->GetTransform()->GetLocalPosition();
		_AllData[_Stage_current].MapDatas[_FieldMap_current].MapObjData[Obj_current].ObjTransform.Scale =
			Parent->AcFieldMaps[_FieldMap_current]->FieldMapObj[Obj_current]->GetTransform()->GetLocalScale();
		_AllData[_Stage_current].MapDatas[_FieldMap_current].MapObjData[Obj_current].ObjTransform.Rot =
			Parent->AcFieldMaps[_FieldMap_current]->FieldMapObj[Obj_current]->GetTransform()->GetLocalRotation();
	}
}
