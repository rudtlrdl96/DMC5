#pragma once
#include <GameEngineCore/GameEngineGUI.h>
#include "StageDatas.h"

class StageEditGUI : public GameEngineGUIWindow
{
	friend class FieldMapObjEditGUI;
public:
	static StageEditGUI* MainEditor;

	StageEditGUI();
	~StageEditGUI();

	StageEditGUI(const StageEditGUI& _Other) = delete;
	StageEditGUI(StageEditGUI&& _Other) noexcept = delete;
	StageEditGUI& operator=(const StageEditGUI& _Other) = delete;
	StageEditGUI& operator=(StageEditGUI&& _Other) noexcept = delete;

	
protected:
	void OnGUI(std::shared_ptr<GameEngineLevel> _Level, float _DeltaTime) override;

private:
	//부모 레벨
	class StageEditLevel* Parent = nullptr;

	//상용 함수
	static std::string GetOpenFilePath(); //파일 불러오기
	static std::string GetSaveFilePath(); //파일 저장하기

	void ShowTransformInfo(std::shared_ptr<GameEngineObject> _Obj = nullptr); // 오브젝트 트랜스폼 툴

	//Data 관련
	std::vector<StageData> AllData;

	void SaveStageData();
	void LoadStageData(std::shared_ptr<GameEngineLevel> _Level);
	void CreateStage(StageData _Data);

	//Stage 관련
	size_t Stage_current = 0;
	char StageNameInputSpace[128] = "";
	void StageCombo(std::shared_ptr<GameEngineLevel> _Level);
	void InputStageInfo(std::shared_ptr<GameEngineLevel> _Level);

	void InputSkyBox();
	void InputMapCol();
	void InputFieldMap();
	
	//FieldMap 관련
	size_t FieldMap_current = 0;
	void FieldMapCombo();
	
	//맵렌더러 관련
	size_t MapParts_current = 0;
	void InputMapParts();
	void MapPartsCombo();

	//맵 컬링 콜리전 관련
	size_t CullingCol_current = 0;
	void InputCullingCol();
	void CullingColCombo();

	//맵 컬링 노드 관련
	bool NodeCheckBool[100] = {};

	void InputNode();
	void NodeCheckBox();

	//맵 오브젝트 관련
	std::shared_ptr<GameEngineGUIWindow> FieldObjEditor = nullptr;



};

