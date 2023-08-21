#include "PrecompileHeader.h"
#include "GameEngineObject.h"
#include "imgui.h"
#include "GameEngineLevel.h"

int GameEngineObject::NextActorID = 0;
int GameEngineObject::GUI_SelectActorIndex = -1;

GameEngineTransform* GameEngineObject::NewChild = nullptr;

GameEngineObject::GameEngineObject() :
	ActorID(NextActorID++)
{
	Transform.SetMaster(this);
}

GameEngineObject::~GameEngineObject() 
{
}


void GameEngineObject::Release() 
{
	Transform.ChildRelease();

	std::list<std::shared_ptr<GameEngineObject>>::iterator ReleaseStartIter = Childs.begin();
	std::list<std::shared_ptr<GameEngineObject>>::iterator ReleaseEndIter = Childs.end();

	for (; ReleaseStartIter != ReleaseEndIter; )
	{
		std::shared_ptr<GameEngineObject>& Object = *ReleaseStartIter;

		if (false == Object->IsDeath())
		{
			++ReleaseStartIter;
			continue;
		}

		ReleaseStartIter = Childs.erase(ReleaseStartIter);
	}
}

void GameEngineObject::AllAccTime(float _DeltaTime)
{
	if (false == IsUpdate())
	{
		return;
	}

	AccLiveTime(_DeltaTime);

	for (std::shared_ptr<GameEngineObject> Object : Childs)
	{
		Object->AllAccTime(_DeltaTime);
	}
}

void GameEngineObject::AllUpdate(float _DeltaTime) 
{
	if (false == IsUpdate())
	{
		return;
	}

	Update(_DeltaTime);

	for (std::shared_ptr<GameEngineObject> Object : Childs)
	{
		Object->AllUpdate(_DeltaTime);
	}
}
void GameEngineObject::AllRender(float _DeltaTime) 
{
	if (false == IsUpdate())
	{
		return;
	}

	Render(_DeltaTime);

	for (std::shared_ptr<GameEngineObject> Object : Childs)
	{
		Object->AllRender(_DeltaTime);
	}

}
void GameEngineObject::AllRelease() 
{
	if (true == IsDeath())
	{
		return;
	}

	Release();

	for (std::shared_ptr<GameEngineObject> Object : Childs)
	{
		Object->AllRelease();
	}

}
bool GameEngineObject::IsDeath() 
{
	GameEngineTransform* Trans = GetTransform()->GetParent();

	if (nullptr != Trans)
	{
		GameEngineObject* Master = Trans->GetMaster();

		return GameEngineObjectBase::IsDeath() || Master->IsDeath();
	}

	return GameEngineObjectBase::IsDeath();
}

bool GameEngineObject::IsUpdate()
{
	GameEngineTransform* Trans = GetTransform()->GetParent();

	if (nullptr != Trans)
	{
		GameEngineObject* Master = Trans->GetMaster();

		return GameEngineObjectBase::IsUpdate() && Master->IsUpdate();
	}

	return GameEngineObjectBase::IsUpdate();
}

void GameEngineObject::AllDestroy() 
{
	if (true == IsDeath())
	{
		Destroy();
	}

	for (std::shared_ptr<GameEngineObject> Object : Childs)
	{
		Object->AllDestroy();
	}
}

void GameEngineObject::AllLevelChangeStart() 
{
	LevelChangeStart();

	for (std::shared_ptr<GameEngineObject> Object : Childs)
	{
		Object->AllLevelChangeStart();
	}
}
void GameEngineObject::AllLevelChangeEnd()
{
	LevelChangeEnd();

	for (std::shared_ptr<GameEngineObject> Object : Childs)
	{
		Object->AllLevelChangeEnd();
	}
}

GameEngineObject* GameEngineObject::DrawGUI()
{
	GameEngineObject* Result = nullptr;

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow |
		ImGuiTreeNodeFlags_OpenOnDoubleClick |
		ImGuiTreeNodeFlags_SpanAvailWidth |
		ImGuiTreeNodeFlags_DefaultOpen;

	if (GUI_SelectActorIndex == GetID())
	{
		flags |= ImGuiTreeNodeFlags_Selected;
	}

	bool isLeaf = false;

	if (Childs.size() == 0)
	{
		flags |= ImGuiTreeNodeFlags_Leaf;
		isLeaf = true;
	}

	const bool treeOpen = ImGui::TreeNodeEx((GetName().data() + std::string(" : ") + std::to_string(GetID())).c_str(), flags);

	// Select entity if clicked
	if (ImGui::IsItemClicked(0))
	{
		Result = this;
		GUI_SelectActorIndex = GetID();
	}

	if (ImGui::BeginDragDropSource())
	{
		NewChild = GetTransform();

		ImGui::SetDragDropPayload("ENTITY", GetTransform(), sizeof(GameEngineTransform*), ImGuiCond_Always);
		ImGui::Text("Drag %s", GetName());
		ImGui::EndDragDropSource();
	}

	bool justDropTarget = false;

	if (ImGui::BeginDragDropTarget())
	{
		const auto* data = ImGui::AcceptDragDropPayload("ENTITY");

		if (data != nullptr && NewChild != nullptr)
		{
			NewChild->SetParent(GetTransform());
			NewChild = nullptr;

			justDropTarget = isLeaf;
		}
		ImGui::EndDragDropTarget();
	}

	if (ImGui::IsItemClicked(1))
	{
		GUI_isRightClickOnItem = true;
		ImGui::OpenPopup("Hierarchy Popup");
	}
	
	if (!GUI_isRightClickOnItem && ImGui::GetIO().MouseClicked[1] && ImGui::IsWindowHovered())
	{
		ImGui::OpenPopup("Hierarchy Popup");
	}

	if (ImGui::BeginPopup("Hierarchy Popup"))
	{
		if (ImGui::Selectable("Remove Entity"))
		{
			Death();
		}

		ImGui::EndPopup();
	}

	if (treeOpen)
	{		
		std::list<std::shared_ptr<GameEngineObject>>::iterator LoopIter = Childs.begin();

		while (LoopIter != Childs.end())
		{
			GameEngineObject* ChildResult = (*LoopIter)->DrawGUI();
			++LoopIter;

			if (nullptr != ChildResult)
			{
				Result = ChildResult;
			}
		}
	}

	ImGui::TreePop();

	return Result;
}