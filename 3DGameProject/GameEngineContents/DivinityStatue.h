#pragma once
#include "StaticFieldMapObject.h"

class DivinityStatue : public StaticFieldMapObject
{
public:
	// constructer destructer
	DivinityStatue();
	~DivinityStatue();

	// delete Function
	DivinityStatue(const DivinityStatue& _Other) = delete;
	DivinityStatue(DivinityStatue&& _Other) = delete;
	DivinityStatue& operator=(const DivinityStatue& _Other) = delete;
	DivinityStatue& operator=(DivinityStatue&& _Other) = delete;

protected:
	void Start() override;

private:

};

