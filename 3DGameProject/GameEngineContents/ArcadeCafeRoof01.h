#pragma once
#include "StaticFieldMapObject.h"
class ArcadeCafeRoof01 : public StaticFieldMapObject
{
public:
	// construtor destructor
	ArcadeCafeRoof01();
	~ArcadeCafeRoof01();

	// delete Function
	ArcadeCafeRoof01(const ArcadeCafeRoof01& _Other) = delete;
	ArcadeCafeRoof01(ArcadeCafeRoof01&& _Other) noexcept = delete;
	ArcadeCafeRoof01& operator=(const ArcadeCafeRoof01& _Other) = delete;
	ArcadeCafeRoof01& operator=(ArcadeCafeRoof01&& _Other) noexcept = delete;

protected:
	void Start() override;

private:

};

