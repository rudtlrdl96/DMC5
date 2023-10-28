#pragma once
#include "StaticFieldMapObject.h"

class l02_03_StreetLine : public StaticFieldMapObject
{
public:
	// construtor destructor
	l02_03_StreetLine();
	~l02_03_StreetLine();

	// delete Function
	l02_03_StreetLine(const l02_03_StreetLine& _Other) = delete;
	l02_03_StreetLine(l02_03_StreetLine&& _Other) noexcept = delete;
	l02_03_StreetLine& operator=(const l02_03_StreetLine& _Other) = delete;
	l02_03_StreetLine& operator=(l02_03_StreetLine&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
private:

};

