#pragma once
#include "StaticFieldMapObject.h"
class RedSeal : public StaticFieldMapObject
{
public:
	// construtor destructor
	RedSeal();
	~RedSeal();

	// delete Function
	RedSeal(const RedSeal& _Other) = delete;
	RedSeal(RedSeal&& _Other) noexcept = delete;
	RedSeal& operator=(const RedSeal& _Other) = delete;
	RedSeal& operator=(RedSeal&& _Other) noexcept = delete;

protected:
	void Start() override;

private:

};

