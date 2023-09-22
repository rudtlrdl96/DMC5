#include "PrecompileHeader.h"
#include "EffectToolWindow.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineFBXAnimation.h>
#include "EffectRenderer.h"
#include "FXSystem.h"
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
	AddKeyFrame();
	Save(Level);
}

void EffectToolWindow::CharacterSetting(std::shared_ptr<GameEngineLevel> Level)
{
	// 캐릭터 매쉬, 애니메이션을 로드하여 적용합니다
	if (ImGui::Button("Open Mesh FBX"))
	{
		OPENFILENAME OFN;
		TCHAR lpstrFile[200] = L"";
		static TCHAR filter[] = L".fbx 메쉬 파일\0*.fbx";

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
				FXSys = Actor->CreateComponent<FXSystem>();
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
		static TCHAR filter[] = L".fbx 애니메이션 파일\0*.fbx";

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
			FXSys = Actor->CreateComponent<FXSystem>();
		}
		std::shared_ptr<EffectRenderer> NewFX = Actor->CreateComponent<EffectRenderer>();
		FXRenders.push_back(NewFX);

		static int FXIndex = 0;
		NewFX->SetName(std::to_string(FXIndex++));
	}
}

void EffectToolWindow::TimeLine()
{
	// 애니메이션의 프레임을 확인하기 위한 Drag와 Button을 지원합니다

	// 프레임 이동기능

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

	if (IsPause)
	{
		if (CharacterRender != nullptr)
		{
			CharacterRender->SetCurFrame(CurrentFrame);
		}
	}

	// 재생, 정지 버튼
	if (ImGui::Button("Play"))
	{
		IsPause = false;
		if (CharacterRender != nullptr)
		{
			CharacterRender->SetPlay();
			CharacterRender->SetCurFrame(0);
		}
		for (std::shared_ptr<EffectRenderer> _Renderer : FXRenders)
		{
			_Renderer->Reset();
		}
		CurUnitDatas.clear();
		CurUnitDatas.resize(FXRenders.size());
		for (int i = 0; i < CurUnitDatas.size(); i++)
		{
			std::shared_ptr<GameEngineFBXMesh> Mesh = FXRenders[i]->GetFBXMesh();
			if (Mesh != nullptr && Mesh->GetName() != "Rect")
			{
				CurUnitDatas[i].MeshName = Mesh->GetName();
				CurUnitDatas[i].TextureName = FXRenders[i]->GetShaderResHelper(0).GetTextureSetter("DiffuseTex")->Res->GetName();
			}
			else
			{
				std::shared_ptr<AnimationInfo> Anim = FXRenders[i]->GetCurAnimation();
				if (nullptr != Anim)
				{
					CurUnitDatas[i].AnimData.AnimationName = Anim->Sprite->GetName();
					CurUnitDatas[i].AnimData.SpriteName = Anim->Sprite->GetName();
					CurUnitDatas[i].AnimData.Start = Anim->StartFrame;
					CurUnitDatas[i].AnimData.End = Anim->EndFrame;
					CurUnitDatas[i].AnimData.Loop = Anim->Loop;
					CurUnitDatas[i].AnimData.FrameInter = Anim->FrameTime[0];
				}
				else
				{
					CurUnitDatas[i].TextureName = FXRenders[i]->GetShaderResHelper(0).GetTextureSetter("DiffuseTex")->Res->GetName();
				}
			}
		}
		if (CurUnitDatas.size() == 0 || CurFrameData.size() == 0)
		{
			return;
		}
		FXSys->SetFX(FXData::CreateData(CurUnitDatas, CurFrameData));
		FXSys->Play();
	}
	ImGui::SameLine();
	if (ImGui::Button("Stop"))
	{
		IsPause = true;
		if (CharacterRender != nullptr)
		{
			CharacterRender->SetPause();
		}
		if (CurUnitDatas.size() == 0 || CurFrameData.size() == 0)
		{
			return;
		}
		FXSys->Pause = true;
	}
}

void EffectToolWindow::AddKeyFrame()
{
	static int UnitIndex = 0;
	ImGui::InputInt("Index", &UnitIndex);
	ImGui::SameLine();

	if (ImGui::Button("AddKeyFrame"))
	{
		FXKeyFrame NewKeyFrame;
		NewKeyFrame.EffectOption = FXRenders[UnitIndex]->EffectOption;
		NewKeyFrame.IsUpdate = FXRenders[UnitIndex]->IsUpdate();
		NewKeyFrame.Position = FXRenders[UnitIndex]->GetTransform()->GetLocalPosition();
		NewKeyFrame.Rotation = FXRenders[UnitIndex]->GetTransform()->GetLocalRotation();
		NewKeyFrame.Scale = FXRenders[UnitIndex]->GetTransform()->GetLocalScale();
		CurFrameData[CurrentFrame].insert(std::pair(UnitIndex, NewKeyFrame));
	}


}

void EffectToolWindow::Save(std::shared_ptr<GameEngineLevel> Level)
{
	if (ImGui::Button("Save Effect"))
	{

		OPENFILENAME OFN;
		TCHAR lpstrFile[200] = L"";
		static TCHAR filter[] = L".effect 이펙트 파일\0*.effect";

		memset(&OFN, 0, sizeof(OPENFILENAME));
		OFN.lStructSize = sizeof(OPENFILENAME);
		OFN.hwndOwner = GameEngineWindow::GetHWnd();
		OFN.lpstrFilter = filter;
		OFN.lpstrFile = lpstrFile;
		OFN.nMaxFile = 200;
		OFN.lpstrInitialDir = L".";

		if (GetSaveFileName(&OFN) != 0) {
			GameEnginePath Path = GameEngineString::UniCodeToAnsi(OFN.lpstrFile);
			if (".effect" != Path.GetExtension())
			{
				Path.SetPath(Path.GetFullPath() + ".effect");
			}
			GameEngineSerializer Ser;

			std::shared_ptr<FXData> Data = FXData::CreateData(CurUnitDatas, CurFrameData);
			Data->Write(Ser);

			unsigned int Size = static_cast<unsigned int>(CurFrameData.size());
			Ser << Size;
			if (Size <= 0)
			{
				return;
			}
			for (std::pair<int, std::map<int, FXKeyFrame>> Pair : CurFrameData)
			{
				Ser << Pair.first;
				Ser << Pair.second;
			}

			GameEngineFile File;
			File.SetPath(Path.GetFullPath());
			File.SaveBin(Ser);
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Load Effect"))
	{

		OPENFILENAME OFN;
		TCHAR lpstrFile[200] = L"";
		static TCHAR filter[] = L".effect 이펙트 파일\0*.effect";

		memset(&OFN, 0, sizeof(OPENFILENAME));
		OFN.lStructSize = sizeof(OPENFILENAME);
		OFN.hwndOwner = GameEngineWindow::GetHWnd();
		OFN.lpstrFilter = filter;
		OFN.lpstrFile = lpstrFile;
		OFN.nMaxFile = 200;
		OFN.lpstrInitialDir = L".";

		if (GetOpenFileName(&OFN) != 0) {
			GameEnginePath Path = GameEngineString::UniCodeToAnsi(OFN.lpstrFile);

			GameEngineFile File;
			GameEngineSerializer Ser;
			File.SetPath(Path.GetFullPath());
			File.LoadBin(Ser);

			FXData Data;
			Data.Read(Ser);

			unsigned int Size = 0;
			Ser >> Size;

			if (Size <= 0)
			{
				return;
			}

			for (unsigned int i = 0; i < Size; ++i)
			{
				std::pair<int, std::map<int, FXKeyFrame>> Pair;

				Ser >> Pair.first;
				Ser >> Pair.second;

				CurFrameData.insert(Pair);
			}

			if (nullptr == Actor)
			{
				Actor = Level->CreateActor<GameEngineActor>();
				FXSys = Actor->CreateComponent<FXSystem>();
			}

			CurUnitDatas = Data.GetUnitDatas();

			for (int i = 0; i < FXRenders.size(); i++)
			{
				FXRenders[i]->Death();
				FXRenders[i] = nullptr;
			}
			FXRenders.resize(CurUnitDatas.size());

			for (int i = 0; i < CurUnitDatas.size(); i++)
			{
				if (CurUnitDatas[i].MeshName != "")
				{
					FXRenders[i] = Actor->CreateComponent<EffectRenderer>();
					FXRenders[i]->SetFBXMesh(CurUnitDatas[i].MeshName, "Effect_2D");
					FXRenders[i]->SetTexture("DiffuseTex", CurUnitDatas[i].TextureName);
					//FXRenders[i]->Off();
				}
				else
				{
					FXRenders[i] = Actor->CreateComponent<EffectRenderer>();
					FXRenders[i]->RectInit("Effect_2D");
					FXRenders[i]->LockRotation();
					if (CurUnitDatas[i].AnimData.AnimationName == "")
					{
						// 애니메이션이 아닌 경우
						FXRenders[i]->SetTexture("DiffuseTex", CurUnitDatas[i].TextureName);
					}
					else
					{
						if (nullptr == FXRenders[i]->FindAnimation(CurUnitDatas[i].AnimData.AnimationName))
						{
							// 애니메이션이 없는 경우
							FXRenders[i]->CreateAnimation(CurUnitDatas[i].AnimData);
						}
						FXRenders[i]->ChangeAnimation(CurUnitDatas[i].AnimData.AnimationName);
					}
					//FXRenders[i]->Off();
				}
			}
		}
	}
}

