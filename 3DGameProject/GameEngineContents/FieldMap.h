#pragma once
#include "BaseStageActor.h"
#include "StageDatas.h"

class FieldMap : public BaseStageActor
{
public:
	// construtor destructor
	FieldMap();
	~FieldMap();

	// delete Function
	FieldMap(const FieldMap& _Other) = delete;
	FieldMap(FieldMap&& _Other) noexcept = delete;
	FieldMap& operator=(const FieldMap& _Other) = delete;
	FieldMap& operator=(FieldMap&& _Other) noexcept = delete;

	static std::shared_ptr<FieldMap> CreateFieldMap(GameEngineLevel* _Level, const std::string_view& _FBXName, const std::vector<FieldMapColData>& _ColVector = std::vector<FieldMapColData>(), const float4& _MapPosition = float4::ZERO);
	
protected:
	void Start();
	void Update(float _DeltaTime);

private:
	std::shared_ptr<class GameEngineFBXRenderer> FieldMapRenderer = nullptr;
	std::vector<std::shared_ptr<class GameEngineCollision>> MapCols;
};

