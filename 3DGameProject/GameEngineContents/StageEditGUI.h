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
	void OnGUI(std::shared_ptr<GameEngineLevel> _Level, float _DeltaTime) override;

private:
	//�θ� ����
	class StageBaseLevel* Parent = nullptr;

	//��� �Լ�
	static std::string GetOpenFilePath(); //���� �ҷ�����
	static std::string GetSaveFilePath(); //���� �����ϱ�

	void ShowTransformInfo(std::shared_ptr<GameEngineObject> _Obj = nullptr); // ������Ʈ Ʈ������ ��

	//Data ����
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
	void LoadStageData(std::shared_ptr<GameEngineLevel> _Level);
	void CreateStage(StageData _Data);
	bool IsCreateStage = false;

	//Stage ����
	size_t Stage_current = 0;
	char StageNameInputSpace[128] = "";
	void StageListBox(std::shared_ptr<GameEngineLevel> _Level);
	void InputStageInfo(std::shared_ptr<GameEngineLevel> _Level);

	//FieldMap ����
	int MapBundleIndex = 0;
	int MapBundle_current = 0;
	size_t FieldMap_current = 0;
	void MapBundleCombo();
	void FieldMapAddButton();
	void FieldMapCombo();
	void FieldMapTransformEditUI(std::shared_ptr<GameEngineObject> _Obj = nullptr);

	//NavMesh ����
	
	
	//Collision ����
	//size_t Collision_current = 0;
	//void FieldMapColList();
};

