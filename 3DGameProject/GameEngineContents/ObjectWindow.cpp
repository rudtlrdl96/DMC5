#include "PrecompileHeader.h"
#include "ObjectWindow.h"

ObjectWindow::ObjectWindow() 
{
}

ObjectWindow::~ObjectWindow() 
{
}

void ObjectWindow::AddObject(std::shared_ptr<GameEngineObject> _Object)
{
	ObjectArray[ObjectSize] = _Object;
	ObjectNames[ObjectSize] = _Object->GetName().data();
	ObjectSize++;
}

void ObjectWindow::OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime)
{
	ObjectSelect();
	if (SelectObject == nullptr)
	{
		return;
	}
	ShowObjectInfo();
	ShowTransformInfo();
}

void ObjectWindow::ObjectSelect()
{
	ImGui::Combo("SelectObject", &ObjectIndex, ObjectNames, ObjectSize);
	if (ObjectArray[ObjectIndex] != nullptr)
	{
		SelectObject = ObjectArray[ObjectIndex];
	}
}

void ObjectWindow::ShowObjectInfo()
{
	// 엑터의 정보 표시 (IsUpdate)
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

void ObjectWindow::ShowTransformInfo()
{
	GameEngineTransform* Transform = SelectObject->GetTransform();
	float4 Position = Transform->GetLocalPosition();
	float4 Rotation = Transform->GetLocalRotation();
	float4 Scale = Transform->GetLocalScale();

	// TransData 표시 및 TransformUpdate를 위한 0위치이동 실행
	ImGui::DragFloat3("Position", Position.Arr1D);
	ImGui::DragFloat3("Rotation", Rotation.Arr1D);
	ImGui::DragFloat3("Scale", Scale.Arr1D);
	
	Transform->SetLocalPosition(Position);
	Transform->SetLocalRotation(Rotation);
	Transform->SetLocalScale(Scale);
}

