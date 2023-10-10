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
static int UnitIndex = 0;

void EffectToolWindow::OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime)
{
	for (int i = 0; i < FXRenders.size(); i++)
	{
		if (FXRenders[i]->IsDeath())
		{
			FXRenders[i] = nullptr;
			FXRenders.erase(FXRenders.begin() + i);

			for (int j = 0; j < FXRenders.size(); j++)
			{
				FXRenders[j]->SetName(std::to_string(j));
			}
		}
	}

	CharacterSetting(Level);
	CreateFXRender(Level);
	TimeLine();
	AddKeyFrame();
	KeyFrame();
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
	if (ImGui::Button("Vergil Mesh FBX"))
	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Character");
		NewDir.Move("Player");
		NewDir.Move("Vergil");
		NewDir.Move("Mesh");
		if (nullptr == GameEngineFBXMesh::Find("Vergil.FBX"))
		{
			GameEngineFBXMesh::Load(NewDir.GetPlusFileName("Vergil.fbx").GetFullPath());
			GameEngineTexture::Load(NewDir.GetPlusFileName("pl0300_03_atos.texout.png").GetFullPath());
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
		CharacterRender->SetFBXMesh("Vergil.FBX", "AniFBX");
		CharacterRender->GetTransform()->SetLocalPosition({ 0, -75, 0 });

		for (int i = 9; i <= 21; i++)
		{
			CharacterRender->GetAllRenderUnit()[0][i]->On();
		}
		for (int i = 2; i <= 4; i++)
		{
			CharacterRender->GetAllRenderUnit()[0][i]->Off();
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
			}
			if (nullptr == CharacterRender->FindAnimation(FileName))
			{
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

		NewFX->SetName(std::to_string(FXRenders.size() - 1));
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
		while (0 < CurrentFrame)
		{
			if (CurFrameData.contains(--CurrentFrame))
			{
				if (CurFrameData[CurrentFrame].contains(UnitIndex))
				{
					break;
				}
			}
		}
	}
	ImGui::SameLine();
	if (ImGui::Button(">>") && CurrentFrame < 9999)
	{
		while (CurrentFrame < 9999)
		{
			if (CurFrameData.contains(++CurrentFrame))
			{
				if (CurFrameData[CurrentFrame].contains(UnitIndex))
				{
					break;
				}
			}
		}
	}

	if (IsPause && BeforeFrame != CurrentFrame)
	{
		if (CharacterRender != nullptr)
		{
			CharacterRender->SetCurFrame(CurrentFrame);
		}
		KeyFramePreview();
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
				CurUnitDatas[i].TextureName = FXRenders[i]->GetShaderResHelper(0).GetTextureSetter("DiffuseTexture")->Res->GetName();
				if (true == FXRenders[i]->IsDistortion())
				{
					CurUnitDatas[i].CutX = 1;
				}
			}
			else
			{
				std::shared_ptr<AnimationInfo> Anim = FXRenders[i]->GetCurAnimation();
				if (nullptr != Anim)
				{
					CurUnitDatas[i].AnimationName = Anim->Sprite->GetName();
				}
				else
				{
					CurUnitDatas[i].TextureName = FXRenders[i]->GetShaderResHelper(0).GetTextureSetter("DiffuseTexture")->Res->GetName();
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
		for (std::shared_ptr<EffectRenderer> _Renderer : FXRenders)
		{
			_Renderer->On();
		}
		FXSys->Pause = true;
		FXSys->Off();
	}
}

void EffectToolWindow::KeyFrame()
{
	if (false == CurFrameData.contains(CurrentFrame)) { return; }

	std::map<int, FXKeyFrame>::iterator StartIter = CurFrameData[CurrentFrame].begin();
	std::map<int, FXKeyFrame>::iterator EndIter = CurFrameData[CurrentFrame].end();

	for (; StartIter != EndIter; StartIter++)
	{

		if (ImGui::TreeNode(std::to_string(StartIter->first).c_str()))
		{
			ImGui::DragFloat4("Position", StartIter->second.Position.Arr1D, 0.1f);
			ImGui::DragFloat4("Rotation", StartIter->second.Rotation.Arr1D, 0.1f);
			ImGui::DragFloat4("Scale", StartIter->second.Scale.Arr1D, 0.1f);

			if (ImGui::Button("Delete"))
			{
				CurFrameData[CurrentFrame].erase(StartIter);
				if (0 == CurFrameData[CurrentFrame].size())
				{
					CurFrameData.erase(CurrentFrame);
				}
				ImGui::TreePop();
				return;
			}

			ImGui::TreePop();
		}

	}
}

void EffectToolWindow::AddKeyFrame()
{
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

		if (true == CurFrameData[CurrentFrame].contains(UnitIndex))
		{
			CurFrameData[CurrentFrame][UnitIndex] = NewKeyFrame;
		}
		else
		{
			CurFrameData[CurrentFrame].insert(std::pair(UnitIndex, NewKeyFrame));
		}
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
			CurFrameData.clear();
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
					FXRenders[i]->SetName(std::to_string(i));
					FXRenders[i]->SetFBXMesh(CurUnitDatas[i].MeshName, "Effect_2D");
					FXRenders[i]->SetTexture("DiffuseTexture", CurUnitDatas[i].TextureName);
					if (CurUnitDatas[i].CutX == 1)
					{
						FXRenders[i]->SetDistortionTexture();
					}
					//FXRenders[i]->Off();
				}
				else
				{
					FXRenders[i] = Actor->CreateComponent<EffectRenderer>();
					FXRenders[i]->SetName(std::to_string(i));
					FXRenders[i]->RectInit("Effect_2D");
					FXRenders[i]->LockRotation();
					if (CurUnitDatas[i].AnimationName == "")
					{
						// 애니메이션이 아닌 경우
						FXRenders[i]->SetTexture("DiffuseTexture", CurUnitDatas[i].TextureName);
					}
					else
					{
						if (nullptr == FXRenders[i]->FindAnimation(CurUnitDatas[i].AnimationName))
						{
							// 애니메이션이 없는 경우
							FXRenders[i]->CreateAnimation({ .AnimationName = CurUnitDatas[i].AnimationName, .SpriteName = CurUnitDatas[i].AnimationName , .FrameInter = 0.0166f });
						}
						FXRenders[i]->ChangeAnimation(CurUnitDatas[i].AnimationName);
					}
					//FXRenders[i]->Off();
				}
			}
		}
	}
}

void EffectToolWindow::KeyFramePreview()
{
	if (false == CurFrameData.contains(CurrentFrame))
	{
		KeyFrameLerp();
		return;
	}

	std::map<int, FXKeyFrame>& CurKeyFrame = CurFrameData[CurrentFrame];
	std::map<int, FXKeyFrame>::iterator StartIter = CurKeyFrame.begin();
	std::map<int, FXKeyFrame>::iterator EndIter = CurKeyFrame.end();

	for (int i = 0; i < FXRenders.size(); i++)
	{
		if (false == CurKeyFrame.contains(i))
		{
			KeyFrameLerp(i);
			continue;
		}

		FXRenders[i]->GetTransform()->SetLocalPosition(CurKeyFrame[i].Position);
		FXRenders[i]->GetTransform()->SetLocalRotation(CurKeyFrame[i].Rotation);
		FXRenders[i]->GetTransform()->SetLocalScale(CurKeyFrame[i].Scale);
		FXRenders[i]->EffectOption = CurKeyFrame[i].EffectOption;

		if (ImGui::TreeNode(std::to_string(StartIter->first).c_str()))
		{
			ImGui::DragFloat4("Position", CurKeyFrame[i].Position.Arr1D, 0.1f);
			ImGui::DragFloat4("Rotation", CurKeyFrame[i].Rotation.Arr1D, 0.1f);
			ImGui::DragFloat4("Scale", CurKeyFrame[i].Scale.Arr1D, 0.1f);

			if (ImGui::Button("Delete"))
			{
				CurFrameData[CurrentFrame].erase(StartIter);
				if (0 == CurFrameData[CurrentFrame].size())
				{
					CurFrameData.erase(CurrentFrame);
				}
				ImGui::TreePop();
				return;
			}

			ImGui::TreePop();
		}
	}

}

void EffectToolWindow::KeyFrameLerp()
{
	for (int i = 0; i < FXRenders.size(); i++)
	{
		FXKeyFrame PrevKey;
		FXKeyFrame NextKey;
		int PrevFrame = CurrentFrame;
		int NextFrame = CurrentFrame;

		bool Check = false;
		while (0 < PrevFrame)
		{
			PrevFrame--;
			if (true == CurFrameData.contains(PrevFrame) && true == CurFrameData[PrevFrame].contains(i))
			{
				if (true == CurFrameData[PrevFrame].contains(i))
				{
					Check = true;
					PrevKey = CurFrameData[PrevFrame][i];
					break;
				}
			}
		}
		if (false == Check) { continue; }

		Check = false;
		while (NextFrame < 9999)
		{
			NextFrame++;
			if (true == CurFrameData.contains(NextFrame) && true == CurFrameData[NextFrame].contains(i))
			{
				if (true == CurFrameData[NextFrame].contains(i))
				{
					Check = true;
					NextKey = CurFrameData[NextFrame][i];
					break;
				}
			}
		}
		if (false == Check) { continue; }

		float Ratio = (float)(CurrentFrame - PrevFrame) / (NextFrame - PrevFrame);
		FXKeyFrame CurKey = FXKeyFrame::Lerp(PrevKey, NextKey, Ratio);
		FXRenders[i]->GetTransform()->SetLocalPosition(CurKey.Position);
		FXRenders[i]->GetTransform()->SetLocalRotation(CurKey.Rotation);
		FXRenders[i]->GetTransform()->SetLocalScale(CurKey.Scale);
		FXRenders[i]->EffectOption = CurKey.EffectOption;
	}
}

void EffectToolWindow::KeyFrameLerp(int i)
{
	FXKeyFrame PrevKey;
	FXKeyFrame NextKey;
	int PrevFrame = CurrentFrame;
	int NextFrame = CurrentFrame;

	bool Check = false;
	while (0 < PrevFrame)
	{
		PrevFrame--;
		if (true == CurFrameData.contains(PrevFrame) && true == CurFrameData[PrevFrame].contains(i))
		{
			if (true == CurFrameData[PrevFrame].contains(i))
			{
				Check = true;
				PrevKey = CurFrameData[PrevFrame][i];
				break;
			}
		}
	}
	if (false == Check) { return; }

	Check = false;
	while (NextFrame < 9999)
	{
		NextFrame++;
		if (true == CurFrameData.contains(NextFrame) && true == CurFrameData[NextFrame].contains(i))
		{
			if (true == CurFrameData[NextFrame].contains(i))
			{
				Check = true;
				NextKey = CurFrameData[NextFrame][i];
				break;
			}
		}
	}
	if (false == Check) { return; }

	float Ratio = (float)(CurrentFrame - PrevFrame) / (NextFrame - PrevFrame);
	FXKeyFrame CurKey = FXKeyFrame::Lerp(PrevKey, NextKey, Ratio);
	FXRenders[i]->GetTransform()->SetLocalPosition(CurKey.Position);
	FXRenders[i]->GetTransform()->SetLocalRotation(CurKey.Rotation);
	FXRenders[i]->GetTransform()->SetLocalScale(CurKey.Scale);
	FXRenders[i]->EffectOption = CurKey.EffectOption;
}

