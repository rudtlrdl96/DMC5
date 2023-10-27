#include "PrecompileHeader.h"
#include "StageBaseLevel.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>

#include "NetworkManager.h"
#include "SkyBox.h"
#include "MapCollisionMesh.h"

#include <GameEngineCore/GameEngineGUI.h>
#include "StageGUI.h"

#include "RedSeal.h"

std::vector<StageData> StageBaseLevel::Location2_StageDatas;
std::vector<StageData> StageBaseLevel::Location11_StageDatas;
std::vector<StageData> StageBaseLevel::Location19_StageDatas;

StageBaseLevel::StageBaseLevel()
{
}

StageBaseLevel::~StageBaseLevel()
{
	if (GUI != nullptr)
	{
		GUI->Death();
		GUI->Off();
	}
}

void StageBaseLevel::LoadAllStageData()
{
	{
		GameEngineSerializer LoadSerializer = GameEngineSerializer();

	GameEngineDirectory Dir;
	Dir.MoveParentToDirectory("ContentData");
	Dir.Move("ContentData");
	Dir.Move("StageData");
	std::filesystem::path temp = Dir.GetPlusFileName("Location2_ver8").GetFullPath();

		GameEngineFile File(temp);
		File.LoadBin(LoadSerializer);

		StageData::ReadAllStageData(LoadSerializer, Location2_StageDatas);
	}

	{
		GameEngineSerializer LoadSerializer = GameEngineSerializer();

		GameEngineDirectory Dir;
		Dir.MoveParentToDirectory("ContentData");
		Dir.Move("ContentData");
		Dir.Move("StageData");
		std::filesystem::path temp = Dir.GetPlusFileName("Location11").GetFullPath();

		GameEngineFile File(temp);
		File.LoadBin(LoadSerializer);

		StageData::ReadAllStageData(LoadSerializer, Location11_StageDatas);
	}

	{
		GameEngineSerializer LoadSerializer = GameEngineSerializer();

		GameEngineDirectory Dir;
		Dir.MoveParentToDirectory("ContentData");
		Dir.Move("ContentData");
		Dir.Move("StageData");
		std::filesystem::path temp = Dir.GetPlusFileName("Location19").GetFullPath();

		GameEngineFile File(temp);
		File.LoadBin(LoadSerializer);

		StageData::ReadAllStageData(LoadSerializer, Location19_StageDatas);
	}

	{
		GameEngineDirectory Dir;
		Dir.MoveParentToDirectory("ContentResources");
		Dir.Move("ContentResources");
		Dir.Move("Sound");
		Dir.Move("SFX");
		Dir.Move("RedSeal");

		if (nullptr == GameEngineSound::Find("RedSeal_0.wav"))
		{
			GameEngineSound::Load(Dir.GetPlusFileName("RedSeal_0.wav").GetFullPath());
			GameEngineSound::Load(Dir.GetPlusFileName("RedSeal_1.wav").GetFullPath());
		}
	}
}

void StageBaseLevel::Start()
{
	BaseLevel::Start();
	CreateScene(GetName());
	if (Location2_StageDatas.empty())
	{
		LoadAllStageData();
	}
}

void StageBaseLevel::Update(float _DeltaTime)
{
	BaseLevel::Update(_DeltaTime);
}

void StageBaseLevel::LevelChangeStart()
{
	LoadStageFBXMesh();
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

void StageBaseLevel::LoadStageFBXMesh()
{
	if (GameEngineFBXMesh::Find("ALPHA_l02_01.fbx") != nullptr)
	{
		return;
	}

	{
		GameEngineDirectory Dir = GameEnginePath::GetFileFullPath
		(
			"ContentResources",
			{
				"Map", "Location2"
			}
		);
		std::vector<GameEngineFile> FBXFiles = Dir.GetAllFile({ ".fbx" });
		for (GameEngineFile& File : FBXFiles)
		{
			GameEngineFBXMesh::Load(File.GetFullPath());
		}
	}
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
		LinkNode(AcFieldMaps[i].lock(), _Data.MapDatas[i].NodeIndex);
		AcFieldMaps[i].lock()->ReflectionSetting();
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

void StageBaseLevel::ReflectionTextureSetting(std::shared_ptr<GameEngineTexture> _RefTexture, const float4& _Pos, const float4& _Scale)
{
	DirectX::BoundingBox ReflectionCol;

	ReflectionCol.Center = _Pos.DirectFloat3;
	ReflectionCol.Extents = _Scale.DirectFloat3;

	for (size_t i = 0; i < AcFieldMaps.size(); i++)
	{
		for (size_t j = 0; j < AcFieldMaps[i].lock()->FieldMapRenderer.size(); j++)
		{
			std::shared_ptr<GameEngineFBXRenderer> MapRenderer = AcFieldMaps[i].lock()->FieldMapRenderer[j].lock();

			GameEngineTransform* MapRendererTrans = MapRenderer->GetTransform();

			DirectX::BoundingBox RendererCol;

			RendererCol.Center = MapRendererTrans->GetWorldPosition().DirectFloat3;
			RendererCol.Extents = DirectX::XMFLOAT3(1, 1, 1);

			if (true == ReflectionCol.Intersects(RendererCol))
			{
				MapRenderer->SetTexture("ReflectionTexture", _RefTexture);
			}
		}
	}
}

void StageBaseLevel::PushBackRedSealWall(std::weak_ptr<class RedSeal> _Wall)
{
	RedSealWalls.emplace_back(_Wall);
}

void StageBaseLevel::RedSealWallOn()
{
	GameEngineSound::Play("RedSeal_0.wav");

	for (size_t i = 0; i < RedSealWalls.size(); i++)
	{
		RedSealWalls[i].lock()->WallOn();
	}
}

void StageBaseLevel::RedSealWallOff()
{
	GameEngineSound::Play("RedSeal_1.wav");

	for (size_t i = 0; i < RedSealWalls.size(); i++)
	{
		RedSealWalls[i].lock()->WallOff();
	}
}

void StageBaseLevel::EraseStageFieldMap()
{
	if (AcFieldMaps.empty())
	{
		return;
	}

	for (size_t i = 0; i < AcFieldMaps.size(); i++)
	{
		AcFieldMaps[i].lock()->EraseFieldMap();
		AcFieldMaps[i].lock()->Death();
		AcFieldMaps[i].lock() = nullptr;
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
		AcFieldMaps[i] = FieldMap::CreateFieldMap(this, _MapDatas[i].PartsMeshFileNames, _MapDatas[i].CullingColTransform, _MapDatas[i].MapObjData);
	}

	const std::vector<std::shared_ptr<GameEngineLight>>& AllLight = GetLevel()->GetAllLightRef();

	for (auto& i : AllLight)
	{
		i->BakeShadow(GetLevel()->GetMainCamera());
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
	if (AcSkyBox.lock() == nullptr)
	{
		return;
	}
	AcSkyBox.lock()->Death();
	AcSkyBox.lock() = nullptr;
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
	if (AcGroundCol.lock() == nullptr)
	{
		return;
	}
	AcGroundCol.lock()->Death();
	AcGroundCol.lock() = nullptr;
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
	if (AcWallCol.lock() == nullptr)
	{
		return;
	}
	AcWallCol.lock()->Death();
	AcWallCol.lock() = nullptr;
}

void StageBaseLevel::LinkNode(std::shared_ptr<FieldMap> _FieldMap, const std::vector<int>& _NodeIndex)
{
	//인덱스를 받는다
	//필드맵 안에 필드맵 여러개를 넣어야하는 상황
	//1. 필드맵의 주소를 받는 벡터를 하나 만든다.
	std::vector<std::weak_ptr<FieldMap>> TempRenderOnNodes;
	std::vector<std::weak_ptr<FieldMap>> TempRenderOffNodes;
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



