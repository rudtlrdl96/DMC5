#pragma once
#include "StaticFieldMapObject.h"
class TentacleE : public StaticFieldMapObject
{
public:
	// construtor destructor
	TentacleE();
	~TentacleE();

	// delete Function
	TentacleE(const TentacleE& _Other) = delete;
	TentacleE(TentacleE&& _Other) noexcept = delete;
	TentacleE& operator=(const TentacleE& _Other) = delete;
	TentacleE& operator=(TentacleE&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:

};

