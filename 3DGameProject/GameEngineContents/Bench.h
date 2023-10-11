#pragma once
#include "StaticFieldMapObject.h"
class Bench : public StaticFieldMapObject
{
public:
	// construtor destructor
	Bench();
	~Bench();

	// delete Function
	Bench(const Bench& _Other) = delete;
	Bench(Bench&& _Other) noexcept = delete;
	Bench& operator=(const Bench& _Other) = delete;
	Bench& operator=(Bench&& _Other) noexcept = delete;

protected:
	void Start() override;

private:

};

