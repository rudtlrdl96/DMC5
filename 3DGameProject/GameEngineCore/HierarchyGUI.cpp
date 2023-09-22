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

	ImGui::Spacing();
	ImGui::Spacing();

	ShowObjectInfo();

	ImGui::Spacing();
	ImGui::Spacing();

	ShowTransformInfo();
}

void HierarchyGUI::ObjectSelect(std::shared_ptr<GameEngineLevel> Level)
{
	std::map<int, std::list<std::shared_ptr<GameEngineActor>>>& AllActors = Level->GetAllActor();

	int OrderCount = 0;

	for (std::pair<const int, std::list<std::shared_ptr<GameEngineActor>>>& RefData : AllActors)
	{
		std::string OrderName = "Order : " + std::to_string(OrderCount);
		ImGui::SeparatorText(OrderName.c_str());

		if (ImGui::TreeNode(OrderName.c_str()))
		{
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

			ImGui::TreePop();
		}
	}

	ImGui::SeparatorText("");
}

void HierarchyGUI::ShowObjectInfo()
{
	//������ ���� ǥ�� (IsUpdate)
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
	float4 PrevPosition = Position;
	float4 Rotation = Transform->GetLocalRotation();
	float4 PrevRotation = Rotation;
	float4 Scale = Transform->GetLocalScale();
	float4 PrevScale = Scale;

	// TransData ǥ�� �� TransformUpdate�� ���� 0��ġ�̵� ����

	ImGui::DragFloat4("Position", Position.Arr1D, 0.1f);
	ImGui::DragFloat4("Rotation", Rotation.Arr1D, 0.1f);
	ImGui::DragFloat4("Scale", Scale.Arr1D, 0.1f);

	if (Position != PrevPosition)
	{
		Transform->SetLocalPosition(Position);
	}
	if (Rotation != PrevRotation)
	{
		Transform->SetLocalRotation(Rotation);
	}
	if (Scale != PrevScale)
	{
		Transform->SetLocalScale(Scale);
	}

	SelectObject->DrawEditor();
}