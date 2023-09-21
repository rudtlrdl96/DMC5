#include "PrecompileHeader.h"
#include "EffectToolWindow.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineFBXAnimation.h>
#include "EffectRenderer.h"

EffectToolWindow::EffectToolWindow() 
{
}

EffectToolWindow::~EffectToolWindow() 
{
}

void EffectToolWindow::OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime)
{
	CharacterSetting(Level);
	CreateFXRender(Level);
	TimeLine();
}

void EffectToolWindow::CharacterSetting(std::shared_ptr<GameEngineLevel> Level)
{
	// ĳ���� �Ž�, �ִϸ��̼��� �ε��Ͽ� �����մϴ�
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
			GameEnginePath Path = GameEngineString::UniCodeToAnsi(OFN.lpstrFile);
			std::string FileName = Path.GetFileName();
			if (nullptr == GameEngineFBXMesh::Find(FileName))
			{
				GameEngineFBXMesh::Load(Path.GetFullPath());
			}

			if (nullptr == Actor)
			{
				Actor = Level->CreateActor<GameEngineActor>();
			}
			if (nullptr != CharacterRender)
			{
				CharacterRender->Death();
			}
			CharacterRender = Actor->CreateComponent<GameEngineFBXRenderer>();
			CharacterRender->SetFBXMesh(FileName, "AniFBX");

			if (FileName == "Nero.fbx" || FileName == "Vergil.fbx")
			{
				CharacterRender->GetTransform()->SetLocalPosition({ 0, -75, 0 });
			}
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
			GameEnginePath Path = GameEngineString::UniCodeToAnsi(OFN.lpstrFile);
			std::string FileName = Path.GetFileName();

			if (nullptr == CharacterRender)
			{
				return;
			}

			if (nullptr == GameEngineFBXAnimation::Find(FileName))
			{
				GameEngineFBXAnimation::Load(Path.GetFullPath());
				CharacterRender->CreateFBXAnimation(FileName, FileName, { .Inter = 0.01666f, .Loop = false });
			}
			CharacterRender->ChangeAnimation(FileName);
			CharacterRender->SetPause();
		}
	}
}

void EffectToolWindow::CreateFXRender(std::shared_ptr<GameEngineLevel> Level)
{
	if (ImGui::Button("New FX Renderer"))
	{
		if (nullptr == Actor)
		{
			Actor = Level->CreateActor<GameEngineActor>();
		}
		std::shared_ptr<EffectRenderer> NewFX = Actor->CreateComponent<EffectRenderer>();
		FXRenders.push_back(NewFX);
	}

	return;
	if (ImGui::Button("Create FBX FX"))
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
			GameEnginePath Path = GameEngineString::UniCodeToAnsi(OFN.lpstrFile);
			std::string FileName = Path.GetFileName();
			if (nullptr == GameEngineFBXMesh::Find(FileName))
			{
				GameEngineFBXMesh::Load(Path.GetFullPath());
			}

			if (nullptr == Actor)
			{
				Actor = Level->CreateActor<GameEngineActor>();
			}
			std::shared_ptr<EffectRenderer> NewFX = Actor->CreateComponent<EffectRenderer>();
			FXRenders.push_back(NewFX);

		}
	}
}

void EffectToolWindow::TimeLine()
{
	// �ִϸ��̼��� �������� Ȯ���ϱ� ���� Drag�� Button�� �����մϴ�
	if (CharacterRender == nullptr) { return; }

	// ������ �̵����

	int BeforeFrame = CurrentFrame;
	ImGui::DragInt("Frame", &CurrentFrame, 1, 0, 9999);
	if (ImGui::Button("<") && 0 < CurrentFrame)
	{
		CurrentFrame--;
	}
	ImGui::SameLine();
	if (ImGui::Button(">") && CurrentFrame < 9999)
	{
		CurrentFrame++;
	}
	ImGui::SameLine();
	if (ImGui::Button("<<") && 0 < CurrentFrame)
	{
		//while (0 < --CurrentFrame)
		//{
		//	if (0 < AnimEvent.Events[CurrentFrame].size())
		//	{
		//		break;
		//	}
		//}
	}
	ImGui::SameLine();
	if (ImGui::Button(">>") && CurrentFrame < 9999)
	{
		//while (++CurrentFrame < FrameSize)
		//{
		//	if (0 < AnimEvent.Events[CurrentFrame].size())
		//	{
		//		break;
		//	}
		//}
	}
	ImGui::SameLine();
	ImGui::Text(std::to_string(CurrentFrame).c_str());

	if (IsPause)
	{
		CharacterRender->SetCurFrame(CurrentFrame);
	}

	// ���, ���� ��ư
	if (ImGui::Button("Play"))
	{
		IsPause = false;
		CharacterRender->SetPlay();
	}
	ImGui::SameLine();
	if (ImGui::Button("Stop"))
	{
		IsPause = true;
		CharacterRender->SetPause();
	}
}

