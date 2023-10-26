#pragma once
#include "StaticFieldMapObject.h"

class ArcadeGate : public StaticFieldMapObject
{
public:
	// construtor destructor
	ArcadeGate();
	~ArcadeGate();

	// delete Function
	ArcadeGate(const ArcadeGate& _Other) = delete;
	ArcadeGate(ArcadeGate&& _Other) noexcept = delete;
	ArcadeGate& operator=(const ArcadeGate& _Other) = delete;
	ArcadeGate& operator=(ArcadeGate&& _Other) noexcept = delete;

protected:
	void Start() override;

private:

};

