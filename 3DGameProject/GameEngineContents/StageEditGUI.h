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

	
protected:
	void OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime) override;

private:
	//상용 함수
	static std::string GetOpenFilePath(); //파일 불러오기
	static std::string GetSaveFilePath(); //파일 저장하기

	//Data 관련
	std::vector<StageData> AllData;
	template <typename T>
	void pushback_Data(std::vector<T>& _Vector)
	{
		_Vector.emplace_back();
	}

	template <typename T>
	void erase_Data(std::vector<T>& _Vector, size_t _current)
	{
		if (_Vector.size() <= _current)
		{
			return;
		}
		_Vector.erase(_Vector.begin() + _current);
	}

	void SaveStageData();
	void LoadStageData();

	//Stage 관련
	size_t Stage_current = 0;
	char StageNameInputSpace[128] = "";
	void StageListBox();
	void InputStageInfo(std::shared_ptr<GameEngineLevel> _Level);

	//FieldMap 관련
	size_t FieldMap_current = 0;
	void FieldMapCombo();

};

