#include "PrecompileHeader.h"
#include "StartStageLevel.h"
#include "NetworkManager.h"
#include <GameEngineCore/GameEngineFBXMesh.h>
#include "FreeCameraActor.h"

#include "JudgementCut.h"
#include "ColorEffect.h"
#include "FXAA_Effect.h"
#include "ZoomEffect.h"

#include "StageBaseLevel.h"

StartStageLevel* StartStageLevel::Inst = nullptr;

StartStageLevel::StartStageLevel()
{
	Inst = this;
}

StartStageLevel::~StartStageLevel()
{
	Inst = nullptr;
}

void StartStageLevel::Start()
{
	GetCamera(0)->GetCamTarget()->CreateEffect<JudgementCut>();
	GetCamera(0)->GetCamTarget()->CreateEffect<FXAA_Effect>();
	GetCamera(0)->GetCamTarget()->CreateEffect<ZoomEffect>();

	StageBaseLevel::Start();

	TempData.MapDatas.resize(1);
	TempData.SkyboxFileName = "SkyBox.fbx";
	TempData.MapDatas[0].MeshFileName = "Location2.fbx";
	TempData.MapDatas[0].FieldMapPosition = float4::ZERO;

	for (size_t i = 0; i < TempData.MapDatas.size(); i++)
	{
		TempData.MapDatas[i].ColDatas.resize(3);
	}

	for (size_t i = 0; i < TempData.MapDatas.size(); i++)
	{
		for (size_t j = 0; j < TempData.MapDatas[i].ColDatas.size(); j++)
		{
			TempData.MapDatas[i].ColDatas[j].Pos = { 100 * static_cast<float>(j), 0, 0, 1 };
			TempData.MapDatas[i].ColDatas[j].Scale = { 100, 100, 100, 1 };
			TempData.MapDatas[i].ColDatas[j].Rot = { 20, 20, 20, 1 };
			TempData.MapDatas[i].ColDatas[j].Type = ColType::OBBBOX2D;
			TempData.MapDatas[i].ColDatas[j].ColOrder = CollisionOrder::Wall;
		}
	}
}

void StartStageLevel::Update(float _DeltaTime)
{
	StageBaseLevel::Update(_DeltaTime);

	/*if (GameEngineInput::IsDown("StageTestKey"))
	{
		TestLoad();
	}*/
}

void StartStageLevel::LevelChangeStart()
{
	{
		std::string Path = GameEnginePath::GetFileFullPath
		(
			"ContentResources",
			{
				"Map", "TestMap"
			},
			"Location2.FBX"
		);

		GameEngineFBXMesh::Load(Path);
	}

	{
		std::string Path = GameEnginePath::GetFileFullPath
		(
			"ContentResources",
			{
				"Map", "TestMap"
			},
			"SkyBox.fbx"
		);

		GameEngineFBXMesh::Load(Path);
	}


	SetCamera({ 0,0,-500 });
	CreateActor<FreeCameraActor>();

	CreateStage(TempData);
	//AllStageDatas.push_back(TempData);
}

void StartStageLevel::TestSave()
{
	GameEngineSerializer SaveSerializer = GameEngineSerializer();
	StageData::WriteAllStageData(SaveSerializer, AllStageDatas);

	GameEnginePath filepath;
	filepath.SetPath(GetSaveFilePath());

	if (filepath.GetFileName() == "")
	{
		return;
	}

	GameEngineFile file = GameEngineFile(std::string_view(filepath.GetFullPath()));
	file.SaveBin(SaveSerializer);
}

void StartStageLevel::TestLoad()
{
	GameEngineSerializer LoadSerializer = GameEngineSerializer();
	
	std::string Temp = GetOpenFilePath();
	std::string_view Temp_view = Temp;
	if (Temp == "")
	{
		return;
	}
	GameEngineFile File(Temp_view);
	File.LoadBin(LoadSerializer);

	StageData::ReadAllStageData(LoadSerializer, AllStageDatas);
}

std::string StartStageLevel::GetOpenFilePath()
{
	OPENFILENAME ofn;       // common dialog box structure
	wchar_t szFile[260];       // buffer for file name
	wchar_t PrevPath[260];
	GetCurrentDirectory(260, PrevPath);

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GameEngineWindow::GetHWnd();
	ofn.lpstrFile = szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = L'\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 

	if (GetOpenFileName(&ofn) == TRUE)
	{
		SetCurrentDirectory(PrevPath);
		return GameEngineString::UniCodeToAnsi(ofn.lpstrFile);
	}

	SetCurrentDirectory(PrevPath);
	return "";
}

std::string StartStageLevel::GetSaveFilePath()
{
	OPENFILENAME ofn;       // common dialog box structure
	wchar_t szFile[260];       // buffer for file name
	wchar_t PrevPath[260];
	GetCurrentDirectory(260, PrevPath);

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GameEngineWindow::GetHWnd();
	ofn.lpstrFile = szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = L'\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 

	if (GetSaveFileName(&ofn) == TRUE)
	{
		SetCurrentDirectory(PrevPath);
		return GameEngineString::UniCodeToAnsi(ofn.lpstrFile);
	}

	SetCurrentDirectory(PrevPath);
	return "";
}


