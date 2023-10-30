#pragma once
#include "StaticFieldMapObject.h"

class FloatingIsland : public StaticFieldMapObject
{
public:
	// construtor destructor
	FloatingIsland();
	~FloatingIsland();

	// delete Function
	FloatingIsland(const FloatingIsland& _Other) = delete;
	FloatingIsland(FloatingIsland&& _Other) noexcept = delete;
	FloatingIsland& operator=(const FloatingIsland& _Other) = delete;
	FloatingIsland& operator=(FloatingIsland&& _Other) noexcept = delete;



protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	
private:

};

