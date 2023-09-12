#include "PrecompileHeader.h"
#include "StageBaseLevel.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>

#include "NetworkManager.h"
#include "SkyBox.h"
#include "MapCollisionMesh.h"

#include <GameEngineCore/GameEngineGUI.h>
#include "StageGUI.h"

std::vector<StageData> StageBaseLevel::AllStageDatas;

StageBaseLevel::StageBaseLevel()
{
}

StageBaseLevel::~StageBaseLevel()
{
}

void StageBaseLevel::LoadAllStageData()
{
	GameEngineSerializer LoadSerializer = GameEngineSerializer();

	GameEngineDirectory Dir;
	Dir.MoveParentToDirectory("ContentData");
	Dir.Move("ContentData");
	Dir.Move("StageData");
	std::filesystem::path temp = Dir.GetPlusFileName("TestStage").GetFullPath();

	GameEngineFile File(temp);
	File.LoadBin(LoadSerializer);

	StageData::ReadAllStageData(LoadSerializer, AllStageDatas);
}

void StageBaseLevel::Start()
{
	BaseLevel::Start();
	CreateScene(GetName());
	if (AllStageDatas.empty())
	{
		LoadAllStageData();
	}

	GetCamera(0)->GetCamTarget()->CreateEffect<JudgementCut>();
	GetCamera(0)->GetCamTarget()->CreateEffect<FXAA_Effect>();
	GetCamera(0)->GetCamTarget()->CreateEffect<ZoomEffect>();
}

void StageBaseLevel::Update(float _DeltaTime)
{
	BaseLevel::Update(_DeltaTime);
}

void StageBaseLevel::LevelChangeStart()
{
	//if (GUI == nullptr)
	//{
	//	GUI = GameEngineGUI::GUIWindowCreate<StageGUI>("StageGUI");
	//}
	//GUI->On();
}

void StageBaseLevel::LevelChangeEnd()
{
	/*GUI->Off();*/
}

void StageBaseLevel::CreateStage(const StageData& _Data)
{
	StageName = _Data.StageName;
	CreateStageFieldMap(_Data.MapDatas);
	CreateSkyBox(_Data.SkyboxFileName);
	CreateGroundCol(_Data.GroundMeshFileName);
	CreateWallCol(_Data.WallMeshFileName);

	if (AcFieldMaps.empty())
	{
		return;
	}

	for (size_t i = 0; i < AcFieldMaps.size(); i++)
	{
		LinkNode(AcFieldMaps[i], _Data.MapDatas[i].NodeIndex);
	}
}

void StageBaseLevel::SetCamera(float4 _Position)
{
	if (GetMainCamera() != nullptr)
	{
		GetMainCamera()->SetProjectionType(CameraType::Perspective);
	}
	GetMainCamera()->GetTransform()->SetLocalPosition(_Position);
}

void StageBaseLevel::EraseStageFieldMap()
{
	if (AcFieldMaps.empty())
	{
		return;
	}

	for (size_t i = 0; i < AcFieldMaps.size(); i++)
	{
		AcFieldMaps[i]->EraseFieldMap();
		AcFieldMaps[i] = nullptr;
	}
	AcFieldMaps.clear();
}

void StageBaseLevel::CreateStageFieldMap(const std::vector<FieldMapData>& _MapDatas)
{
	if (_MapDatas.empty())
	{
		return;
	}

	AcFieldMaps.resize(_MapDatas.size());
	for (size_t i = 0; i < AcFieldMaps.size(); i++)
	{
		AcFieldMaps[i] = FieldMap::CreateFieldMap(this, _MapDatas[i].PartsMeshFileNames, _MapDatas[i].CullingColTransform);
	}
}

void StageBaseLevel::CreateSkyBox(const std::string_view& _MeshFileName)
{
	if (_MeshFileName == "\0")
	{
		return;
	}

	AcSkyBox = SkyBox::CreateSkyBox(this, _MeshFileName);
}

void StageBaseLevel::EraseSkyBox()
{
	if (AcSkyBox == nullptr)
	{
		return;
	}
	AcSkyBox->Death();
	AcSkyBox = nullptr;
}

void StageBaseLevel::CreateGroundCol(const std::string_view& _MeshFileName)
{
	if (_MeshFileName == "\0")
	{
		return;
	}
	AcGroundCol = MapCollisionMesh::CreateGroundCollisionMesh(this, _MeshFileName);
}

void StageBaseLevel::EraseGroundCol()
{
	if (AcGroundCol == nullptr)
	{
		return;
	}
	AcGroundCol->Death();
	AcGroundCol = nullptr;
}

void StageBaseLevel::ClearStage()
{
	if (StageName == "")
	{
		return;
	}
	StageName = "";
	EraseStageFieldMap();
	EraseSkyBox();
	EraseGroundCol();
	EraseWallCol();
}

void StageBaseLevel::CreateWallCol(const std::string_view& _MeshFileName)
{
	if (_MeshFileName == "\0")
	{
		return;
	}
	AcWallCol = MapCollisionMesh::CreateWallCollisionMesh(this, _MeshFileName);
}

void StageBaseLevel::EraseWallCol()
{
	if (AcWallCol == nullptr)
	{
		return;
	}
	AcWallCol->Death();
	AcWallCol = nullptr;
}

void StageBaseLevel::LinkNode(std::shared_ptr<FieldMap> _FieldMap, const std::vector<int>& _NodeIndex)
{
	//인덱스를 받는다
	//필드맵 안에 필드맵 여러개를 넣어야하는 상황
	//1. 필드맵의 주소를 받는 벡터를 하나 만든다.
	std::vector<std::shared_ptr<FieldMap>> TempRenderOnNodes;
	std::vector<std::shared_ptr<FieldMap>> TempRenderOffNodes;
	std::vector<int> TempOffNodeIndex;

	if (AcFieldMaps.empty())
	{
		return;
	}

	for (int i = 0; i < AcFieldMaps.size(); i++)
	{
		bool IsSame = false;
		for (int j = 0; j < _NodeIndex.size(); j++)
		{
			if (_NodeIndex[j] == i)
			{
				IsSame = true;
				break;
			}
		}

		if (!IsSame)
		{
			TempOffNodeIndex.push_back(i);
		}
	}

	
	//2. 노드 인덱스와 같은 인덱스를 가진 필드맵을 TempNodes에 넣는다
	TempRenderOnNodes.reserve(_NodeIndex.size());
	for (size_t i = 0; i < TempRenderOnNodes.capacity(); i++)
	{
		TempRenderOnNodes.push_back(AcFieldMaps[_NodeIndex[i]]);
	}
	
	TempRenderOffNodes.reserve(TempOffNodeIndex.size());
	for (size_t i = 0; i < TempRenderOffNodes.capacity(); i++)
	{
		TempRenderOffNodes.push_back(AcFieldMaps[TempOffNodeIndex[i]]);
	}

	//3. 
	_FieldMap->PushNode(TempRenderOnNodes, TempRenderOffNodes);
}



