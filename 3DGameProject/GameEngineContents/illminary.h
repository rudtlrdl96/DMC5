#pragma once
#include "StaticFieldMapObject.h"

class illminary : public StaticFieldMapObject
{
public:
	// construtor destructor
	illminary();
	~illminary();

	// delete Function
	illminary(const illminary& _Other) = delete;
	illminary(illminary&& _Other) noexcept = delete;
	illminary& operator=(const illminary& _Other) = delete;
	illminary& operator=(illminary&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:

};

