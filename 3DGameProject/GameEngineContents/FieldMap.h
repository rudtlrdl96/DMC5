#pragma once
#include "BaseStageActor.h"
#include "StageDatas.h"

class FieldMap : public BaseStageActor
{
	friend class StageBaseLevel;
	friend class StageEditGUI;
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
		const std::vector<ObjTransformData>& _CullingCols
		//const std::vector<int>& _NodeIndex
	);

	void PushNode(const std::vector<std::shared_ptr<FieldMap>>& _RenderOn, const std::vector<std::shared_ptr<FieldMap>>& _RenderOff);

	void EraseFieldMap();



protected:
	void DrawEditor() override;
	void Update(float _DeltaTime) override;

private:
	std::vector<std::shared_ptr<class GameEngineFBXRenderer>> FieldMapRenderer;
	std::vector<std::shared_ptr<class GameEngineCollision>> FieldMapCullingCol;
	std::vector<std::weak_ptr<FieldMap>> RenderOnNode;
	std::vector<std::weak_ptr<FieldMap>> RenderOffNode;
	
	void ClearFieldMapRenderer();
	void ClearFieldMapCullingCol();
	void ClearRenderNode();

	bool IsPlayerCollsionToCullingCol();

	void FieldMapRendererOn();
	void FieldMapRendererOff();

	void MapCulling();
};

