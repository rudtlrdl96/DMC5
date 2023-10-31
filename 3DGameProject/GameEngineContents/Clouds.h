#pragma once
#include "StaticFieldMapObject.h"
class Clouds : public StaticFieldMapObject
{
public:
	// construtor destructor
	Clouds();
	~Clouds();

	// delete Function
	Clouds(const Clouds& _Other) = delete;
	Clouds(Clouds&& _Other) noexcept = delete;
	Clouds& operator=(const Clouds& _Other) = delete;
	Clouds& operator=(Clouds&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:

};

