#include "PrecompileHeader.h"
#include "GameEngineObject.h"

#include "imgui.h"
#include "GameEngineLevel.h"
#include "PhysicsActor.h"
#include "PhysXBoxComponent.h"
#include "PhysXSphereComponent.h"
#include "PhysXCapsuleComponent.h"
#include "PhysXTriangleComponent.h"

int GameEngineObject::NextActorID = 0;
int GameEngineObject::GUI_SelectActorIndex = -1;

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
		GameEngineActor* Actor = this->DynamicThis<GameEngineActor>().get();

		if (nullptr != Actor && true == Actor->PhysicsUpdate)
		{
			for (std::shared_ptr<GameEngineObject> Object : Childs)
			{
				PhysXBoxComponent* BoxComponent = Object->DynamicThis<PhysXBoxComponent>().get();
				PhysXSphereComponent* SphereComponent = Object->DynamicThis<PhysXSphereComponent>().get();
				PhysXCapsuleComponent* CapsuleComponent = Object->DynamicThis<PhysXCapsuleComponent>().get();
				PhysXTriangleComponent* TriangleComponent = Object->DynamicThis<PhysXTriangleComponent>().get();

				if (nullptr != BoxComponent)
				{
					BoxComponent->Off();
					Actor->PhysicsUpdate = false;
				}
				if (nullptr != SphereComponent)
				{
					SphereComponent->Off();
					Actor->PhysicsUpdate = false;
				}
				if (nullptr != CapsuleComponent)
				{
					CapsuleComponent->Off();
					Actor->PhysicsUpdate = false;
				}
				if (nullptr != TriangleComponent)
				{
					TriangleComponent->Off();
					Actor->PhysicsUpdate = false;
				}
			}
		}
		return;
	}
	else
	{
		std::shared_ptr<GameEngineActor> Actor = this->DynamicThis<GameEngineActor>();

		if (nullptr != Actor && false == Actor->PhysicsUpdate)
		{
			for (std::shared_ptr<GameEngineObject> Object : Childs)
			{
				PhysXBoxComponent* BoxComponent = Object->DynamicThis<PhysXBoxComponent>().get();
				PhysXSphereComponent* SphereComponent = Object->DynamicThis<PhysXSphereComponent>().get();
				PhysXCapsuleComponent* CapsuleComponent = Object->DynamicThis<PhysXCapsuleComponent>().get();
				PhysXTriangleComponent* TriangleComponent = Object->DynamicThis<PhysXTriangleComponent>().get();

				if (nullptr != BoxComponent)
				{
					BoxComponent->On();
					Actor->PhysicsUpdate = true;
				}
				if (nullptr != SphereComponent)
				{
					SphereComponent->Off();
					Actor->PhysicsUpdate = false;
				}
				if (nullptr != CapsuleComponent)
				{
					CapsuleComponent->On();
					Actor->PhysicsUpdate = true;
				}
				if (nullptr != TriangleComponent)
				{
					TriangleComponent->On();
					Actor->PhysicsUpdate = true;
				}
			}
		}
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
		std::shared_ptr<PhysicsActor> m_pPhysicsActor = this->DynamicThis<PhysicsActor>();

		if (nullptr != m_pPhysicsActor)
		{
			m_pPhysicsActor->Release();
			m_pPhysicsActor = nullptr;
		}

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
		ImGuiTreeNodeFlags_SpanAvailWidth;

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

	// ���õǾ����� üũ
	if (ImGui::IsItemClicked(0))
	{
		Result = this;
		GUI_SelectActorIndex = GetID();
	}

	// �巡�� ����
	//if (ImGui::BeginDragDropSource())
	//{
	//	ImGui::SetDragDropPayload("ENTITY", this, sizeof(GameEngineObject*), ImGuiCond_Always);
	//	ImGui::Text("Drag %d", GetID());
	//	ImGui::EndDragDropSource();
	//}
	//
	//bool justDropTarget = false;
	//
	//if (ImGui::BeginDragDropTarget())
	//{
	//	const auto* data = ImGui::AcceptDragDropPayload("ENTITY");
	//
	//	if (data != nullptr)
	//	{
	//		GameEngineObject* Data = (GameEngineObject*)(data->Data);
	//
	//		// ���� ���� �θ�� �ٲٷ��� ���� ��� ����ó��
	//		bool EqaulChildCheck = false;
	//
	//		for (std::shared_ptr<GameEngineObject>& Ref : Childs)
	//		{
	//			if (Ref->GetID() == Data->GetID())
	//			{
	//				EqaulChildCheck = true;
	//				break;
	//			}
	//		}
	//
	//		if (false == EqaulChildCheck)
	//		{
	//			// �θ� ��ȯ
	//			Data->GetTransform()->SetParent(GetTransform());
	//			justDropTarget = isLeaf;
	//		}
	//	}
	//
	//	ImGui::EndDragDropTarget();
	//}

	// ��Ŭ�� ���� ��ȯ
	if (ImGui::IsItemClicked(1))
	{
		ClickedObject = this;
		ImGui::OpenPopup("Hierarchy Popup");
	}
	
	// ���� ��Ŭ�� ���� ��� Popup Open
	if (this == ClickedObject && ImGui::GetIO().MouseClicked[1] && ImGui::IsWindowHovered())
	{
		ImGui::OpenPopup("Hierarchy Popup");
	}

	if (this == ClickedObject && ImGui::BeginPopup("Hierarchy Popup"))
	{
		// ������ ������ �ڽ����� ���� ����
		if (ImGui::Selectable("Create Entity"))
		{
			std::shared_ptr<GameEngineActor> NewActor = GetLevel()->CreateActor<GameEngineActor>();
			NewActor->GetTransform()->SetParent(GetTransform());
		}
		// ������ ���� ����
		else if (ImGui::Selectable("Remove Entity"))
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

		ImGui::TreePop();
	}

	return Result;
}