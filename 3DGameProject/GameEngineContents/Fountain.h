#pragma once
#include "StaticFieldMapObject.h"
class Fountain : public StaticFieldMapObject
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

private:

};

