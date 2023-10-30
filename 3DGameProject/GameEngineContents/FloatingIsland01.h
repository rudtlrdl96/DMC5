#pragma once
#include "StaticFieldMapObject.h"

class FloatingIsland01 : public StaticFieldMapObject
{
public:
	// construtor destructor
	FloatingIsland01();
	~FloatingIsland01();

	// delete Function
	FloatingIsland01(const FloatingIsland01& _Other) = delete;
	FloatingIsland01(FloatingIsland01&& _Other) noexcept = delete;
	FloatingIsland01& operator=(const FloatingIsland01& _Other) = delete;
	FloatingIsland01& operator=(FloatingIsland01&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:

};

