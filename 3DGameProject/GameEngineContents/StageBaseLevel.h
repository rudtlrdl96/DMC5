#pragma once
#include "StageDatas.h"
#include "BaseLevel.h"
#include "FieldMap.h"

class StageBaseLevel : public BaseLevel
{
	friend class StageEditGUI;
public:

	StageBaseLevel();
	~StageBaseLevel();
	StageBaseLevel(const StageBaseLevel& _Other) = delete;
	StageBaseLevel(StageBaseLevel&& _Other) noexcept = delete;
	StageBaseLevel& operator=(const StageBaseLevel& _Other) = delete;
	StageBaseLevel& operator=(StageBaseLevel&& _Other) noexcept = delete;

	void SetCamera(float4 _Position);
	
protected:
	static std::vector<StageData> AllStageDatas;
	static void LoadAllStageData();

	void Start() override;
	void Update(float _DeltaTime) override;
	
	void LevelChangeStart() override;

	std::string StageName = "";
	std::map<int, std::vector<std::shared_ptr<class FieldMap>>> AcFieldMaps;
	std::shared_ptr<class SkyBox> AcSkyBox = nullptr;
	std::shared_ptr<class MapCollisionMesh> AcGroundCol = nullptr;
	std::shared_ptr<class MapCollisionMesh> AcWallCol = nullptr;

	void CreateStage(const StageData& _Data); // 데이터 넣어주면 스테이지 생성

private:
	//std::shared_ptr<class ContentGrid> DebugGrid = nullptr;

	void CreateStageFieldMap(const std::map<int, std::vector<FieldMapData>>& _MapDatas);
	void EraseStageFieldMap(int _mapbundleindex = -1, int _mapindex = -1);
	void CreateSkyBox(const std::string_view& _MeshFileName);
	void EraseSkyBox();
	void CreateGroundCol(const std::string_view& _MeshFileName);
	void EraseGroundCol();
	void CreateWallCol(const std::string_view& _MeshFileName);
	void EraseWallCol();

	void ClearStage();
};

