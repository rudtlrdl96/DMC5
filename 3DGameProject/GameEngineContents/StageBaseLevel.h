#pragma once
#include "StageDatas.h"
#include "BaseLevel.h"
#include "FieldMap.h"

class StageBaseLevel : public BaseLevel
{
public:
	static std::vector<StageData> AllStageDatas;

	StageBaseLevel();
	~StageBaseLevel();
	StageBaseLevel(const StageBaseLevel& _Other) = delete;
	StageBaseLevel(StageBaseLevel&& _Other) noexcept = delete;
	StageBaseLevel& operator=(const StageBaseLevel& _Other) = delete;
	StageBaseLevel& operator=(StageBaseLevel&& _Other) noexcept = delete;


	void StartProcess(GameEngineLevel* Level);
	void SetCamera(float4 _Position);
	
protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	std::shared_ptr<class FieldMap> AcFieldMap = nullptr;

private:

};

