#include "PrecompileHeader.h"
#include "GameEngineObject.h"
#include "imgui.h"
#include "GameEngineLevel.h"

int GameEngineObject::NextActorID = 0;
int GameEngineObject::GUI_SelectActorIndex = -1;
bool GameEngineObject::GUI_CurFrameSetParent = false;

GameEngineTransform* GameEngineObject::NewChild = nullptr;
GameEngineObject* GameEngineObject::ClickedObject = nullptr;

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

	// 선택되었는지 체크
	if (ImGui::IsItemClicked(0))
	{
		Result = this;
		NewChild = GetTransform();
		GUI_SelectActorIndex = GetID();
	}

	// 드래그 시작
	if (ImGui::BeginDragDropSource())
	{
		ImGui::SetDragDropPayload("ENTITY", GetTransform(), sizeof(GameEngineTransform*), ImGuiCond_Always);
		ImGui::Text("Drag %d", GetID());
		ImGui::EndDragDropSource();
	}

	bool justDropTarget = false;

	if (ImGui::BeginDragDropTarget())
	{
		const auto* data = ImGui::AcceptDragDropPayload("ENTITY");

		if (data != nullptr && NewChild != nullptr)
		{
			// 만약 같은 부모로 바꾸려고 했을 경우 예외처리
			bool EqaulChildCheck = false;

			for (std::shared_ptr<GameEngineObject>& Ref : Childs)
			{
				if (Ref->GetID() == NewChild->GetMaster()->GetID())
				{
					EqaulChildCheck = true;
					break;
				}
			}

			if (false == EqaulChildCheck)
			{
				// 부모 변환
				NewChild->SetParent(GetTransform());
				NewChild = nullptr;

				GUI_CurFrameSetParent = true;

				justDropTarget = isLeaf;
			}
		}
		ImGui::EndDragDropTarget();
	}

	// 우클릭 여부 반환
	if (ImGui::IsItemClicked(1))
	{
		ClickedObject = this;
		ImGui::OpenPopup("Hierarchy Popup");
	}
	
	// 만약 우클릭 했을 경우 Popup Open
	if (this == ClickedObject && ImGui::GetIO().MouseClicked[1] && ImGui::IsWindowHovered())
	{
		ImGui::OpenPopup("Hierarchy Popup");
	}

	if (this == ClickedObject && ImGui::BeginPopup("Hierarchy Popup"))
	{
		// 선택한 액터의 자식으로 액터 생성
		if (ImGui::Selectable("Create Entity"))
		{
			std::shared_ptr<GameEngineActor> NewActor = GetLevel()->CreateActor<GameEngineActor>();
			NewActor->GetTransform()->SetParent(GetTransform());
		}
		// 선택한 액터 삭제
		else if (ImGui::Selectable("Remove Entity"))
		{
			Death();
		}

		ImGui::EndPopup();
	}

	if (treeOpen)
	{		
		std::list<std::shared_ptr<GameEngineObject>>::iterator LoopIter = Childs.begin();

		while (LoopIter != Childs.end() && false == GUI_CurFrameSetParent)
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