#pragma once
#include "StaticFieldMapObject.h"
class Spike : public StaticFieldMapObject
{
public:
	// construtor destructor
	Spike();
	~Spike();

	// delete Function
	Spike(const Spike& _Other) = delete;
	Spike(Spike&& _Other) noexcept = delete;
	Spike& operator=(const Spike& _Other) = delete;
	Spike& operator=(Spike&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:

};

