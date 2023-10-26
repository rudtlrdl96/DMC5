#pragma once
#include "StaticFieldMapObject.h"

class ArcadeCafeRoof02 : public StaticFieldMapObject
{
public:
	// construtor destructor
	ArcadeCafeRoof02();
	~ArcadeCafeRoof02();

	// delete Function
	ArcadeCafeRoof02(const ArcadeCafeRoof02& _Other) = delete;
	ArcadeCafeRoof02(ArcadeCafeRoof02&& _Other) noexcept = delete;
	ArcadeCafeRoof02& operator=(const ArcadeCafeRoof02& _Other) = delete;
	ArcadeCafeRoof02& operator=(ArcadeCafeRoof02&& _Other) noexcept = delete;

protected:
	void Start() override;

private:

};

