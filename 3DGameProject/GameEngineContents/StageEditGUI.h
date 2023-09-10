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
	class StageEditLevel* Parent = nullptr;

	//��� �Լ�
	static std::string GetOpenFilePath(); //���� �ҷ�����
	static std::string GetSaveFilePath(); //���� �����ϱ�

	void ShowTransformInfo(std::shared_ptr<GameEngineObject> _Obj = nullptr); // ������Ʈ Ʈ������ ��

	//Data ����
	std::vector<StageData> AllData;

	void SaveStageData();
	void LoadStageData(std::shared_ptr<GameEngineLevel> _Level);
	void CreateStage(StageData _Data);

	//Stage ����
	size_t Stage_current = 0;
	char StageNameInputSpace[128] = "";
	void StageListBox(std::shared_ptr<GameEngineLevel> _Level);
	void InputStageInfo(std::shared_ptr<GameEngineLevel> _Level);

	void InputSkyBox();
	void InputMapCol();
	void InputFieldMap();
	
	//FieldMap ����
	size_t FieldMap_current = 0;
	void FieldMapCombo();
	
	//�ʷ����� ����
	size_t MapParts_current = 0;
	void InputMapParts();
	void MapPartsCombo();

	//�� �ø� �ݸ��� ����
	size_t CullingCol_current = 0;
	void InputCullingCol();
	void CullingColCombo();

	//�� �ø� ��� ����
	std::vector<bool> NodeCheckBool;

	void InputNode();
	void NodeCheckBox();

};

