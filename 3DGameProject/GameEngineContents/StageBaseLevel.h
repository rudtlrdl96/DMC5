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

	//void FieldMapOn(int _Index = -1);
	//void FieldMapOff(int _Index = -1);
	
protected:
	static std::vector<StageData> AllStageDatas;
	static void LoadAllStageData();
	std::shared_ptr<class GameEngineGUIWindow> GUI = nullptr;

	void Start() override;
	void Update(float _DeltaTime) override;
	
	void LevelChangeStart() override;
	void LevelChangeEnd() override;

	std::string StageName = "";
	std::vector<std::shared_ptr<class FieldMap>> AcFieldMaps;
	std::shared_ptr<class SkyBox> AcSkyBox = nullptr;
	std::shared_ptr<class MapCollisionMesh> AcGroundCol = nullptr;
	std::shared_ptr<class MapCollisionMesh> AcWallCol = nullptr;

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

