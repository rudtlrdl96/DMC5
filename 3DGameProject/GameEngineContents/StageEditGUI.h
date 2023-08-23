#pragma once
#include <GameEngineCore/GameEngineGUI.h>
#include "StageDatas.h"

class StageEditGUI : public GameEngineGUIWindow
{
public:
	StageEditGUI();
	~StageEditGUI();

	StageEditGUI(const StageEditGUI& _Other) = delete;
	StageEditGUI(StageEditGUI&& _Other) noexcept = delete;
	StageEditGUI& operator=(const StageEditGUI& _Other) = delete;
	StageEditGUI& operator=(StageEditGUI&& _Other) noexcept = delete;

	StageData StageDatas;
	FieldMapData FieldMapDatas;
	FieldMapColData FieldMapColDatas;

	const char* StageNameSet(int _NumverIndex);
protected:
	void OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime) override;
private:
	bool BoxSwitch=false;
	int EditBoxSize = 12;
	int current = 1;

	std::vector<const char*> StageNumber;
	int NumberIndex = 0;

	std::vector<std::string> PreChar;
	int StartPrecharSize = 20;
};

