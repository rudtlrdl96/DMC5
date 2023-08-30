#include "PrecompileHeader.h"
#include "AnimationToolWindow.h"
//#include <fstream>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineFBXAnimation.h>
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include "AnimationEvent.h"

AnimationToolWindow::AnimationToolWindow()
{
	PreviewRenderer.reserve(10);
}

AnimationToolWindow::~AnimationToolWindow()
{

}

void AnimationToolWindow::OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime)
{
	FileLoad(Level);
	AnimationTimeLine();
	FrameEvent();
	PreviewObject();
	FileSave();
	Release();
}

void AnimationToolWindow::FileLoad(std::shared_ptr<GameEngineLevel> _Level)
{
	// FBX ����, �Ǵ� Animation ������ �ҷ��� �̺�Ʈ ������ �����մϴ�

	if (ImGui::Button("Open Mesh FBX"))
	{
		OPENFILENAME OFN;
		TCHAR lpstrFile[200] = L"";
		static TCHAR filter[] = L".fbx �޽� ����\0*.fbx";

		memset(&OFN, 0, sizeof(OPENFILENAME));
		OFN.lStructSize = sizeof(OPENFILENAME);
		OFN.hwndOwner = GameEngineWindow::GetHWnd();
		OFN.lpstrFilter = filter;
		OFN.lpstrFile = lpstrFile;
		OFN.nMaxFile = 200;
		OFN.lpstrInitialDir = L".";

		if (GetOpenFileName(&OFN) != 0) {
			MeshFilePath.SetPath(GameEngineString::UniCodeToAnsi(OFN.lpstrFile));
			std::string FileName = MeshFilePath.GetFileName();
			size_t Upper = FileName.find(".FBX");
			size_t Lower = FileName.find(".fbx");
			if (Upper != -1)
			{
				MeshName = FileName.substr(0, Upper);
			}
			else if (Lower != -1)
			{
				MeshName = FileName.substr(0, Lower);
			}
			else
			{
				MsgAssert("FBX������ Ȯ���� ����. ��ҹ��ڸ� Ȯ�����ּ���.");
			}
			MeshName += ".FBX";
			if (nullptr == GameEngineFBXMesh::Find(MeshName))
			{
				GameEngineFBXMesh::Load(MeshFilePath.GetFullPath());
			}
			// FBX �޽��� �ε�� �ڵ����� �ش� FBX�� ������ ����
			MeshCreate(_Level);
			int a = 0;
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Open Animation FBX"))
	{
		OPENFILENAME OFN;
		TCHAR lpstrFile[200] = L"";
		static TCHAR filter[] = L".fbx �ִϸ��̼� ����\0*.fbx";

		memset(&OFN, 0, sizeof(OPENFILENAME));
		OFN.lStructSize = sizeof(OPENFILENAME);
		OFN.hwndOwner = GameEngineWindow::GetHWnd();
		OFN.lpstrFilter = filter;
		OFN.lpstrFile = lpstrFile;
		OFN.nMaxFile = 200;
		OFN.lpstrInitialDir = L".";

		if (GetOpenFileName(&OFN) != 0) {
			AnimEvent.Clear();
			AnimFBXFilePath.SetPath(GameEngineString::UniCodeToAnsi(OFN.lpstrFile));
			AnimationFBXName = AnimFBXFilePath.GetFileName();
			std::string AnimFile = AnimFBXFilePath.GetFullPath();
			size_t Upper = AnimationFBXName.find(".FBX");
			size_t Lower = AnimationFBXName.find(".fbx");
			size_t PathUpper = AnimFile.find(".FBX");
			size_t PathLower = AnimFile.find(".FBX");

			if (Upper != -1)
			{
				AnimationName = AnimationFBXName.substr(0, Upper);
				AnimFile = AnimFile.substr(0, PathUpper);
			}
			else if (Lower != -1)
			{
				AnimationName = AnimationFBXName.substr(0, Lower);
				AnimFile = AnimFile.substr(0, PathLower);
			}
			else
			{
				MsgAssert("FBX������ Ȯ���� ����. ��ҹ��ڸ� Ȯ�����ּ���.");
			}
			if (nullptr == GameEngineFBXAnimation::Find(AnimationFBXName))
			{
				GameEngineFBXAnimation::Load(AnimFBXFilePath.GetFullPath());
			}
			// FBX �ִϸ��̼��� �ε�� �ڵ����� �ش� FBX�� �������� �ִϸ��̼� ����
			AnimationCreate(_Level);
			AnimFile += ".animation";
			AnimFilePath.SetPath(AnimFile);
		}
	}
	ImGui::SameLine();
	// ���� ����
	if (ImGui::Button("Open Animation"))
	{
		// ������ ���� .animation ������ �ҷ����� ���
		OPENFILENAME OFN;
		TCHAR lpstrFile[200] = L"";
		static TCHAR filter[] = L".animation �ִϸ��̼� ����\0*.animation";

		memset(&OFN, 0, sizeof(OPENFILENAME));
		OFN.lStructSize = sizeof(OPENFILENAME);
		OFN.hwndOwner = GameEngineWindow::GetHWnd();
		OFN.lpstrFilter = filter;
		OFN.lpstrFile = lpstrFile;
		OFN.nMaxFile = 200;
		OFN.lpstrInitialDir = L".";
		if (GetOpenFileName(&OFN) != 0) {
			AnimEvent.Clear();
			AnimFilePath.SetPath(GameEngineString::UniCodeToAnsi(OFN.lpstrFile));

			GameEngineSerializer Ser;
			GameEngineFile File;
			File.SetPath(AnimFilePath.GetFullPath());
			File.LoadBin(Ser);
			AnimEvent.Read(Ser);

			if (Renderer == nullptr)
			{
				return;
			}
			if (false == Renderer->Animations.contains(GameEngineString::ToUpper(AnimEvent.AnimationName + ".FBX")))
			{
				std::string pathstr = GameEnginePath::GetFolderPath(AnimFilePath.GetFullPath()) + AnimEvent.AnimationName + ".fbx";
				AnimFBXFilePath.SetPath(pathstr);
				AnimationFBXName = AnimFBXFilePath.GetFileName();
				size_t Upper = AnimationFBXName.find(".FBX");
				size_t Lower = AnimationFBXName.find(".fbx");

				if (Upper != -1)
				{
					AnimationName = AnimationFBXName.substr(0, Upper);
				}
				else if (Lower != -1)
				{
					AnimationName = AnimationFBXName.substr(0, Lower);
				}
				else
				{
					MsgAssert("FBX������ Ȯ���� ����. ��ҹ��ڸ� Ȯ�����ּ���.");
				}
				if (nullptr == GameEngineFBXAnimation::Find(AnimationFBXName))
				{
					GameEngineFBXAnimation::Load(AnimFBXFilePath.GetFullPath());
				}
				// FBX �ִϸ��̼��� �ε�� �ڵ����� �ش� FBX�� �������� �ִϸ��̼� ����
				AnimationCreate(_Level);
			}
			else
			{
				Renderer->ChangeAnimation(AnimEvent.AnimationName);
			}
		}
	}
}

void AnimationToolWindow::AnimationTimeLine()
{
	// �ִϸ��̼��� �������� Ȯ���ϱ� ���� Drag�� Button�� �����մϴ�
	if (Renderer == nullptr) { return; }
	if (Renderer->CurAnimation == nullptr) { return; }

	// ������ �̵����

	int BeforeFrame = CurrentFrame;
	ImGui::DragInt("Frame", &CurrentFrame, 1, 0, FrameSize);
	if (ImGui::Button("<") && 0 < CurrentFrame)
	{
		CurrentFrame--;
	}
	ImGui::SameLine();
	if (ImGui::Button(">") && CurrentFrame < FrameSize)
	{
		CurrentFrame++;
	}
	ImGui::SameLine();
	ImGui::Text((" (" + std::to_string(CurrentFrame) + " / " + std::to_string(FrameSize) + ")").c_str());

	// �ִϸ��̼� �ӵ� ����
	ImGui::DragFloat("Speed", &AnimEvent.Speed, 0.1f, 0.0f, 20.0f);
	if (IsStop)
	{
		Renderer->CurAnimation->CurFrame = CurrentFrame;
	}
	else
	{
		CurrentFrame = Renderer->CurAnimation->CurFrame;
		Renderer->CurAnimation->TimeScale = AnimEvent.Speed;
	}
	if (BeforeFrame != CurrentFrame)
	{
		AnimationFrameUpdate();
	}

	// ���, ���� ��ư
	if (ImGui::Button("Play"))
	{
		IsStop = false;
		Renderer->CurAnimation->TimeScale = AnimEvent.Speed;
	}
	ImGui::SameLine();
	if (ImGui::Button("Stop"))
	{
		IsStop = true;
		Renderer->CurAnimation->TimeScale = 0;
	}
}

void AnimationToolWindow::FrameEvent()
{
	if (Renderer == nullptr) { return; }
	if (Renderer->CurAnimation == nullptr) { return; }

	// ���� �����ӿ� ���ο� �̺�Ʈ�� �����ϴ� ��ư
	if (ImGui::Button("Add Object Event"))
	{
		AnimEvent.Events[CurrentFrame].push_back(EventData(EventType::ObjectUpdate));
	}
	if (ImGui::Button("Add CallBack Event"))
	{
		AnimEvent.Events[CurrentFrame].push_back(EventData(EventType::CallBackVoid));
	}

	// ���� �����ӿ� �����ϴ� �̺�Ʈ�� ǥ���ϰ�, ���ڰ��� �����Ѵ�
	std::vector<EventData>::iterator Iter = AnimEvent.Events[CurrentFrame].begin();
	std::vector<EventData>::iterator End = AnimEvent.Events[CurrentFrame].end();
	int i = 0;
	for (; Iter != End; Iter++)
	{
		EventData& CurData = (*Iter);
		std::string EventName = std::to_string(i++) + ". ";

		switch (CurData.Type)
		{
		case EventType::None:
			EventName += "None";
			break;
		case EventType::ObjectUpdate:
			EventName += "ObjectUpdate";
			break;
		case EventType::CallBackVoid:
			EventName += "CallBackVoid";
			break;
		case EventType::CallBackInt:
			EventName += "CallBackInt";
			break;
		case EventType::CallBackFloat:
			EventName += "CallBackFloat";
			break;
		default:
			break;
		}

		if (ImGui::TreeNode(EventName.c_str()))
		{
			ImGui::InputInt("Index", &CurData.Index);
			if (CurData.Type == EventType::ObjectUpdate)
			{
				ObjUpdateEvent(CurData);
			}
			else
			{
				CallEvent(CurData);
			}

			if (ImGui::Button("Delete Event"))
			{
				AnimEvent.Events[CurrentFrame].erase(Iter);
				ImGui::TreePop();
				return;
			}

			ImGui::TreePop();
		}
	}
}

void AnimationToolWindow::ObjUpdateEvent(EventData& _Data)
{
	ImGui::DragFloat4("Position", _Data.Position.Arr1D);
	ImGui::DragFloat4("Rotation", _Data.Rotation.Arr1D);
	ImGui::DragFloat4("Scale", _Data.Scale.Arr1D);

	std::shared_ptr<GameEngineRenderer> Renderer = nullptr;
	if (0 <= _Data.Index && _Data.Index < PreviewRenderer.size())
	{
		Renderer = PreviewRenderer[_Data.Index];
		GameEngineTransform* Transform = Renderer->GetTransform();
		Transform->SetLocalPosition(_Data.Position);
		Transform->SetLocalRotation(_Data.Rotation);
		Transform->SetLocalScale(_Data.Scale);
	}


	if (_Data.IsUpdate == true)
	{
		if (ImGui::Button("IsUpdate : true"))
		{
			_Data.IsUpdate = false;
			if (nullptr != Renderer)
			{
				Renderer->Off();
			}
		}
	}
	else
	{
		if (ImGui::Button("IsUpdate : false"))
		{
			_Data.IsUpdate = true;
			if (nullptr != Renderer)
			{
				Renderer->On();
			}
		}
	}
}

void AnimationToolWindow::CallEvent(EventData& _Data)
{
	if (ImGui::BeginCombo("CallBack Type", "Change Data Type", 0))
	{
		if (ImGui::Selectable("void"))
		{
			_Data.Type = EventType::CallBackVoid;
		}
		if (ImGui::Selectable("int"))
		{
			_Data.Type = EventType::CallBackInt;
		}
		if (ImGui::Selectable("float"))
		{
			_Data.Type = EventType::CallBackFloat;
		}
		ImGui::EndCombo();
	}
	if (_Data.Type == EventType::CallBackInt)
	{
		ImGui::InputInt("Value", &_Data.IntValue);
	}
	else if (_Data.Type == EventType::CallBackFloat)
	{
		ImGui::InputFloat("Value", &_Data.FloatValue);
	}
}

void AnimationToolWindow::FileSave()
{
	// �ϼ��� �ִϸ��̼� �̺�Ʈ�� �����մϴ�
	if (AnimationName != "" && ImGui::Button("Save Animation"))
	{
		GameEngineSerializer Ser;
		AnimEvent.Write(Ser);
		GameEngineFile File;
		File.SetPath(AnimFilePath.GetFullPath());
		File.SaveBin(Ser);
	}
}

void AnimationToolWindow::MeshCreate(std::shared_ptr<GameEngineLevel> _Level)
{
	// FBX �޽� �ε�� �������� �����Ѵ�
	if (nullptr != Renderer)
	{
		Renderer->Death();
		Renderer = nullptr;
	}
	if (Actor != nullptr)
	{
		if (Actor->GetLevel() != _Level.get())
		{
			Actor->Death();
			Actor = nullptr;
		}
	}
	if (Actor == nullptr)
	{
		Actor = _Level->CreateActor<GameEngineActor>();
	}

	Renderer = Actor->CreateComponent<GameEngineFBXRenderer>();

	if (MeshName != "")
	{
		Renderer->SetFBXMesh(MeshName, "MeshAniTexture");
	}
}

void AnimationToolWindow::AnimationCreate(std::shared_ptr<GameEngineLevel> Level)
{
	// FBX �ִ� �ε�� �ִϸ� �����Ѵ�
	if (nullptr == Renderer)
	{
		return;
	}

	std::string UpperName = GameEngineString::ToUpper(AnimationName);
	if (Renderer->Animations.end() == Renderer->Animations.find(UpperName))
	{
		Renderer->CreateFBXAnimation(UpperName, AnimationFBXName);
	}
	AnimEvent.AnimationName = AnimationName;
	Renderer->ChangeAnimation(UpperName);
	FrameSize = Renderer->CurAnimation->End - 1;
	Renderer->CurAnimation->TimeScale = 0;
}

void AnimationToolWindow::AnimationFrameUpdate()
{
	// ������ ȭ�鿡�� �ִϸ��̼� �̺�Ʈ�� ������ ǥ���ϴ� �Լ�(�������� ����� �������ݴϴ�)
	std::vector<EventData>::iterator Iter = AnimEvent.Events[CurrentFrame].begin();
	std::vector<EventData>::iterator End = AnimEvent.Events[CurrentFrame].end();
	for (; Iter != End; Iter++)
	{
		EventData& CurData = (*Iter);
		if (CurData.Type != EventType::ObjectUpdate) { continue; }
		if (CurData.Index == -1) { continue; }
		if (PreviewRenderer.size() < CurData.Index + 1) { continue; }
		std::shared_ptr<GameEngineRenderer> Renderer = PreviewRenderer[CurData.Index];
		GameEngineTransform* Transform = Renderer->GetTransform();
		Transform->SetLocalPosition(CurData.Position);
		Transform->SetLocalRotation(CurData.Rotation);
		Transform->SetLocalScale(CurData.Scale);
		if (true == CurData.IsUpdate)
		{
			Renderer->On();
		}
		else
		{
			Renderer->Off();
		}
	}
}

void AnimationToolWindow::PreviewObject()
{
	// ���ο� ������ ������Ʈ�� �����ϴ� ���
	if (nullptr == Actor) { return; }
	if (ImGui::Button("Add Preview"))
	{
		std::shared_ptr<GameEngineRenderer> NewPreviewRenderer = Actor->CreateComponent<GameEngineRenderer>();
		PreviewRenderer.push_back(NewPreviewRenderer);
		NewPreviewRenderer->GetTransform()->SetLocalScale({ 100, 100, 100 });
		std::shared_ptr <GameEngineRenderUnit> Unit = NewPreviewRenderer->CreateRenderUnit();
		Unit->SetMesh("DebugBox");
		Unit->SetMaterial("FBX");
	}
}

void AnimationToolWindow::Release()
{
	// ������ ����, ������ ���� �����ϴ� ���
	if (ImGui::Button("Release"))
	{
		Actor->Death();
		Actor = nullptr;
		Renderer->Death();
		Renderer = nullptr;
		for (std::shared_ptr<GameEngineRenderer> _Render : PreviewRenderer)
		{
			_Render->Death();
		}
		PreviewRenderer.clear();
	}
}

