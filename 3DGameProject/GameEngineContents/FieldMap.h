#pragma once
#include "BaseStageActor.h"
#include "StageDatas.h"

class FieldMap : public BaseStageActor
{
	friend class StageBaseLevel;
	friend class StageEditGUI;
	friend class FieldMapObjEditGUI;
public:
	// construtor destructor
	FieldMap();
	~FieldMap();

	// delete Function
	FieldMap(const FieldMap& _Other) = delete;
	FieldMap(FieldMap&& _Other) noexcept = delete;
	FieldMap& operator=(const FieldMap& _Other) = delete;
	FieldMap& operator=(FieldMap&& _Other) noexcept = delete;

	static std::shared_ptr<FieldMap> CreateFieldMap
	(
		GameEngineLevel* _Level,
		const std::vector<std::string>& _FBXNames,
		const std::vector<ObjTransformData>& _CullingCols,
		const std::vector<FieldMapObjData>& _FieldMapObjs
	);

	void PushNode(const std::vector<std::weak_ptr<FieldMap>>& _RenderOn, const std::vector<std::weak_ptr<FieldMap>>& _RenderOff);
	void EraseFieldMap();
	void ReflectionSetting();

protected:
	void DrawEditor() override;
	void Update(float _DeltaTime) override;

private:
	std::vector<std::weak_ptr<class GameEngineFBXRenderer>> FieldMapRenderer;
	std::vector<std::weak_ptr<class GameEngineCollision>> FieldMapCullingCol;
	std::vector<std::weak_ptr<FieldMap>> RenderOnNode;
	std::vector<std::weak_ptr<FieldMap>> RenderOffNode;
	std::vector<std::weak_ptr<class FieldMapObject>> FieldMapObj;
	std::list<std::weak_ptr<class FieldMapObject>> CullingObj;
	std::weak_ptr<class ReflectionSetter> Reflection;
	
	void ClearFieldMapRenderer();
	void ClearFieldMapCullingCol();
	void ClearRenderNode();
	void ClearFieldMapObj();

	bool IsPlayerCollsionToCullingCol();

	void FieldMapRendererOn();
	void FieldMapRendererOff();
	void FieldMapObjOn();
	void FieldMapObjOff();

	void MapCulling();
};

