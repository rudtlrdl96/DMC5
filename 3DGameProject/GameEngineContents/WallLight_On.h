#pragma once
#include "StaticFieldMapObject.h"

class WallLight_On : public StaticFieldMapObject
{
public:
	// construtor destructor
	WallLight_On();
	~WallLight_On();

	// delete Function
	WallLight_On(const WallLight_On& _Other) = delete;
	WallLight_On(WallLight_On&& _Other) noexcept = delete;
	WallLight_On& operator=(const WallLight_On& _Other) = delete;
	WallLight_On& operator=(WallLight_On&& _Other) noexcept = delete;

protected:
	void Start() override;

private:
	std::shared_ptr<class GameEngineLight> PointLight = nullptr;
};

