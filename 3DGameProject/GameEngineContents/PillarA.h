#pragma once
#include "StaticFieldMapObject.h"
class PillarA : public StaticFieldMapObject
{
public:
	// construtor destructor
	PillarA();
	~PillarA();

	// delete Function
	PillarA(const PillarA& _Other) = delete;
	PillarA(PillarA&& _Other) noexcept = delete;
	PillarA& operator=(const PillarA& _Other) = delete;
	PillarA& operator=(PillarA&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:

};

