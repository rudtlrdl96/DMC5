#pragma once
#include "StaticFieldMapObject.h"
class CliffA : public StaticFieldMapObject
{
public:
	// construtor destructor
	CliffA();
	~CliffA();

	// delete Function
	CliffA(const CliffA& _Other) = delete;
	CliffA(CliffA&& _Other) noexcept = delete;
	CliffA& operator=(const CliffA& _Other) = delete;
	CliffA& operator=(CliffA&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
private:

};

