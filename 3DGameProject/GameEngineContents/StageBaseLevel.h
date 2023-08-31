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

	std::string StageName = "";
	std::vector<std::shared_ptr<class FieldMap>> AcFieldMaps;
	std::shared_ptr<class SkyBox> AcSkyBox = nullptr;
	std::shared_ptr<class NavMesh> AcNavMesh = nullptr;

	void CreateStage(const StageData& _Data); // ������ �־��ָ� �������� ����

private:
	void CreateStageFieldMap(const std::vector<FieldMapData>& _MapDatas);
	void EraseStageFieldMap(int _index = -1);
	void CreateSkyBox(const std::string_view& _MeshFileName);
	void EraseSkyBox();
	void CreateNavMesh(const std::string_view& _MeshFileName);
	void EraseNavMesh();

	void ClearStage();
};

