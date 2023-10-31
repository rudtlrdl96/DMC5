#pragma once
#include "StaticFieldMapObject.h"
class TentacleB : public StaticFieldMapObject
{
public:
	// construtor destructor
	TentacleB();
	~TentacleB();

	// delete Function
	TentacleB(const TentacleB& _Other) = delete;
	TentacleB(TentacleB&& _Other) noexcept = delete;
	TentacleB& operator=(const TentacleB& _Other) = delete;
	TentacleB& operator=(TentacleB&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:

};

