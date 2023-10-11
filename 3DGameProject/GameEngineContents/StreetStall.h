#pragma once
#include "StaticFieldMapObject.h"

class StreetStall : public StaticFieldMapObject
{
public:
	// construtor destructor
	StreetStall();
	~StreetStall();

	// delete Function
	StreetStall(const StreetStall& _Other) = delete;
	StreetStall(StreetStall&& _Other) noexcept = delete;
	StreetStall& operator=(const StreetStall& _Other) = delete;
	StreetStall& operator=(StreetStall&& _Other) noexcept = delete;

protected:
	void Start() override;

private:

};

