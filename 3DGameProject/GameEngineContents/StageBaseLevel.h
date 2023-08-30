#pragma once
#include "StageDatas.h"
#include "BaseLevel.h"
#include "FieldMap.h"

class StageBaseLevel : public BaseLevel
{
	friend class StageEditGUI;
public:
	static std::vector<StageData> AllStageDatas;

	StageBaseLevel();
	~StageBaseLevel();
	StageBaseLevel(const StageBaseLevel& _Other) = delete;
	StageBaseLevel(StageBaseLevel&& _Other) noexcept = delete;
	StageBaseLevel& operator=(const StageBaseLevel& _Other) = delete;
	StageBaseLevel& operator=(StageBaseLevel&& _Other) noexcept = delete;

	void SetCamera(float4 _Position);
	
protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	std::string StageName = "";
	std::vector<std::shared_ptr<class FieldMap>> AcFieldMaps;
	std::shared_ptr<class SkyBox> AcSkyBox = nullptr;

	void CreateStage(const StageData& _Data); // 데이터 넣어주면 스테이지 생성

private:
	void CreateStageFieldMap(const std::vector<FieldMapData>& _MapDatas);
	void EraseStageFieldMap(int _index);
	void CreateSkyBox(const std::string_view& _MeshFileName);
	void EraseSkyBox();

	void ClearStage();
};

