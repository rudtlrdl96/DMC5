#pragma once

#include "AutoResourceLoader.h"

// Ό³Έν :
class DynamicResourcesLoadTest : public GameEngineLevel
{
public:
	// constrcuter destructer
	DynamicResourcesLoadTest();
	~DynamicResourcesLoadTest();

	// delete Function
	DynamicResourcesLoadTest(const DynamicResourcesLoadTest& _Other) = delete;
	DynamicResourcesLoadTest(DynamicResourcesLoadTest&& _Other) noexcept = delete;
	DynamicResourcesLoadTest& operator=(const DynamicResourcesLoadTest& _Other) = delete;
	DynamicResourcesLoadTest& operator=(DynamicResourcesLoadTest&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void LevelChangeStart() override;
	void LevelChangeEnd() override;

private:
	AutoResourceLoader Loader;

};

