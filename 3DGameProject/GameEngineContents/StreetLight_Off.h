#pragma once
#include "StaticFieldMapObject.h"
class StreetLight_Off : public StaticFieldMapObject
{
public:
	// construtor destructor
	StreetLight_Off();
	~StreetLight_Off();

	// delete Function
	StreetLight_Off(const StreetLight_Off& _Other) = delete;
	StreetLight_Off(StreetLight_Off&& _Other) noexcept = delete;
	StreetLight_Off& operator=(const StreetLight_Off& _Other) = delete;
	StreetLight_Off& operator=(StreetLight_Off&& _Other) noexcept = delete;

protected:
	void Start() override;

private:

};

