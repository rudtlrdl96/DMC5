#pragma once
#include "FieldMapObject.h"
#include <GameEngineCore/GameEngineFSM.h>

enum class PortalState
{
	Apear,
	Idle
};

class l19_Portal : public FieldMapObject
{
public:
	// construtor destructor
	l19_Portal();
	~l19_Portal();

	// delete Function
	l19_Portal(const l19_Portal& _Other) = delete;
	l19_Portal(l19_Portal&& _Other) noexcept = delete;
	l19_Portal& operator=(const l19_Portal& _Other) = delete;
	l19_Portal& operator=(l19_Portal&& _Other) noexcept = delete;

	void On() override;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	std::shared_ptr<class LevelChangeZone> AcLevelChangeZone = nullptr;
	float Time = 0.f;
	float Ratio = 0.f;
	float4 StartPos = {0,-1000.f,0};
	float4 EndPos = { 0,-85.f,0 };
};

