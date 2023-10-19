#pragma once
#include "StaticFieldMapObject.h"

class StatueB : public StaticFieldMapObject
{
public:
	// construtor destructor
	StatueB();
	~StatueB();

	// delete Function
	StatueB(const StatueB& _Other) = delete;
	StatueB(StatueB&& _Other) noexcept = delete;
	StatueB& operator=(const StatueB& _Other) = delete;
	StatueB& operator=(StatueB&& _Other) noexcept = delete;

protected:
	void Start() override;

private:

};

