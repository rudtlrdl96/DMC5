#pragma once
#include "StageDatas.h"
#include "BaseLevel.h"
#include "FieldMap.h"

class StageBaseLevel : public BaseLevel
{
	friend class StageEditGUI;
	friend class FieldMapObjEditGUI;
public:

	StageBaseLevel();
	~StageBaseLevel();
	StageBaseLevel(const StageBaseLevel& _Other) = delete;
	StageBaseLevel(StageBaseLevel&& _Other) noexcept = delete;
	StageBaseLevel& operator=(const StageBaseLevel& _Other) = delete;
	StageBaseLevel& operator=(StageBaseLevel&& _Other) noexcept = delete;

	void SetCamera(float4 _Position);

	void ReflectionTextureSetting(std::shared_ptr<GameEngineTexture> _RefTexture, const float4& _Pos, const float4& _Scale);
	void PushBackRedSealWall(std::weak_ptr<class RedSeal> _Wall);

	void RedSealWallOn();
	void RedSealWallOff();

	inline std::shared_ptr<class BasePlayerActor> GetMyPlayer() const
	{
		return MyPlayer;
	}

	bool IsEditLevel = false;
	
protected:
	std::shared_ptr<class BasePlayerActor> MyPlayer = nullptr;

	static std::vector<StageData> Location2_StageDatas;
	static std::vector<StageData> Location11_StageDatas;
	static std::vector<StageData> Location19_StageDatas;
	static void LoadAllStageData();
	std::shared_ptr<class GameEngineGUIWindow> GUI = nullptr;

	void Start() override;
	void Update(float _DeltaTime) override;
	
	void LevelChangeStart() override;
	void LevelChangeEnd() override;

	std::string StageName = "";
	std::vector<std::weak_ptr<class FieldMap>> AcFieldMaps;
	std::weak_ptr<class SkyBox> AcSkyBox;
	std::weak_ptr<class MapCollisionMesh> AcGroundCol;
	std::weak_ptr<class MapCollisionMesh> AcWallCol;


	std::vector<std::weak_ptr<class RedSeal>> RedSealWalls;

	float4 PlayerStartPos = float4::ZERO;

	//�������� ����
	void LoadStageFBXMesh();
	void CreateStage(const StageData& _Data); // ������ �־��ָ� �������� ����

private:
	//std::shared_ptr<class ContentGrid> DebugGrid = nullptr;

	void CreateStageFieldMap(const std::vector<FieldMapData>& _MapDatas);
	void EraseStageFieldMap();
	void CreateSkyBox(const std::string_view& _MeshFileName);
	void EraseSkyBox();
	void CreateGroundCol(const std::string_view& _MeshFileName);
	void EraseGroundCol();
	void CreateWallCol(const std::string_view& _MeshFileName);
	void EraseWallCol();
	void LinkNode(std::shared_ptr<class FieldMap> _FieldMap, const std::vector<int>& _NodeIndex);

	void ClearStage();
};

