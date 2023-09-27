#pragma once
#include "StaticFieldMapObject.h"

class WallLight_Off : public StaticFieldMapObject
{
public:
	// construtor destructor
	WallLight_Off();
	~WallLight_Off();

	// delete Function
	WallLight_Off(const WallLight_Off& _Other) = delete;
	WallLight_Off(WallLight_Off&& _Other) noexcept = delete;
	WallLight_Off& operator=(const WallLight_Off& _Other) = delete;
	WallLight_Off& operator=(WallLight_Off&& _Other) noexcept = delete;

protected:
	void Start() override;

private:

};

