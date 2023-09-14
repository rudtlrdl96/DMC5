#pragma once
#include "FieldMapObject.h"

class Fountain : public FieldMapObject
{
public:
	// construtor destructor
	Fountain();
	~Fountain();

	// delete Function
	Fountain(const Fountain& _Other) = delete;
	Fountain(Fountain&& _Other) noexcept = delete;
	Fountain& operator=(const Fountain& _Other) = delete;
	Fountain& operator=(Fountain&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:

};

