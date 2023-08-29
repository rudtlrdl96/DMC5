#pragma once
#include "GameEngineActor.h"
#include "GameEngineRenderer.h"

struct GridData
{
	float4 GridScale;
};

// Ό³Έν :
class EngineGrid : public GameEngineActor
{
public:
	// constrcuter destructer
	EngineGrid();
	~EngineGrid();

	// delete Function
	EngineGrid(const EngineGrid& _Other) = delete;
	EngineGrid(EngineGrid&& _Other) noexcept = delete;
	EngineGrid& operator=(const EngineGrid& _Other) = delete;
	EngineGrid& operator=(EngineGrid&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _Delta) override;
	void Render(float _Delta) override;

private:
	GridData GridDataInst;


	float4 MinGridInter = { 100.0f, 5.0f, 100.0f, };

	float4 MinSize = { 10000.0f, 1.0f, 10000.0f };

	float Inter = 100.0f;

	std::shared_ptr<class GameEngineRenderer> Renderer = nullptr;
};

