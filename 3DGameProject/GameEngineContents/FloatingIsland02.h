#pragma once
#include "StaticFieldMapObject.h"
class FloatingIsland02 : public StaticFieldMapObject
{
public:
	// construtor destructor
	FloatingIsland02();
	~FloatingIsland02();

	// delete Function
	FloatingIsland02(const FloatingIsland02& _Other) = delete;
	FloatingIsland02(FloatingIsland02&& _Other) noexcept = delete;
	FloatingIsland02& operator=(const FloatingIsland02& _Other) = delete;
	FloatingIsland02& operator=(FloatingIsland02&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:

};

