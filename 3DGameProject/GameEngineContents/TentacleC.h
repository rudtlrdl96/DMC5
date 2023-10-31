#pragma once
#include "StaticFieldMapObject.h"
class TentacleC : public StaticFieldMapObject
{
public:
	// construtor destructor
	TentacleC();
	~TentacleC();

	// delete Function
	TentacleC(const TentacleC& _Other) = delete;
	TentacleC(TentacleC&& _Other) noexcept = delete;
	TentacleC& operator=(const TentacleC& _Other) = delete;
	TentacleC& operator=(TentacleC&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:

};

