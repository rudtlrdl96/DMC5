#pragma once
#include "StaticFieldMapObject.h"
class PillarB : public StaticFieldMapObject
{
public:
	// construtor destructor
	PillarB();
	~PillarB();

	// delete Function
	PillarB(const PillarB& _Other) = delete;
	PillarB(PillarB&& _Other) noexcept = delete;
	PillarB& operator=(const PillarB& _Other) = delete;
	PillarB& operator=(PillarB&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:

};

