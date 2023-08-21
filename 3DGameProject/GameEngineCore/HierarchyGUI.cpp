#include "PrecompileHeader.h"
#include "HierarchyGUI.h"
#include "GameEngineLevel.h"

HierarchyGUI::HierarchyGUI() 
{
}

HierarchyGUI::~HierarchyGUI() 
{
}

void HierarchyGUI::OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime)
{
	if (nullptr == Level)
	{
		return;
	}

	ObjectSelect(Level);

	if (SelectObject == nullptr)
	{
		return;
	}

	if (true == SelectObject->IsDeath())
	{
		SelectObject = nullptr;
		return;
	}

	ShowObjectInfo();
	ShowTransformInfo();
}

void HierarchyGUI::ObjectSelect(std::shared_ptr<GameEngineLevel> Level)
{
	std::map<int, std::list<std::shared_ptr<GameEngineActor>>>& AllActors = Level->GetAllActor();

	int OrderCount = 0;

	for (std::pair<const int, std::list<std::shared_ptr<GameEngineActor>>>& RefData : AllActors)
	{
		ImGui::Text("Order : %d", OrderCount);

		std::list<std::shared_ptr<GameEngineActor>>& OrderActors = RefData.second;
		std::list<std::shared_ptr<GameEngineActor>>::iterator LoopIter = OrderActors.begin();;

		while (LoopIter != OrderActors.end())
		{
			GameEngineObject* Result = (*LoopIter)->DrawGUI();

			if (nullptr != Result)
			{
				SelectObject = Result;
			}

			++LoopIter;
		}
	}
}

void HierarchyGUI::ShowObjectInfo()
{
	//엑터의 정보 표시 (IsUpdate)
	if (SelectObject->IsUpdate() == true)
	{
		if (ImGui::Button("IsUpdate : true"))
		{
			SelectObject->Off();
		}
	}
	else
	{
		if (ImGui::Button("IsUpdate : false"))
		{
			SelectObject->On();
		}
	}
}

void HierarchyGUI::ShowTransformInfo()
{
	GameEngineTransform* Transform = SelectObject->GetTransform();
	float4 Position = Transform->GetLocalPosition();
	float4 Rotation = Transform->GetLocalRotation();
	float4 Scale = Transform->GetLocalScale();

	// TransData 표시 및 TransformUpdate를 위한 0위치이동 실행

	ImGui::DragFloat4("Position", Position.Arr1D);
	ImGui::DragFloat4("Rotation", Rotation.Arr1D);
	ImGui::DragFloat4("Scale", Scale.Arr1D);

	Transform->SetLocalPosition(Position);
	Transform->SetLocalRotation(Rotation);
	Transform->SetLocalScale(Scale);
}