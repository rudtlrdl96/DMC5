#pragma once
#include "StaticFieldMapObject.h"
class StreetLight_On : public StaticFieldMapObject
{
public:
	// construtor destructor
	StreetLight_On();
	~StreetLight_On();

	// delete Function
	StreetLight_On(const StreetLight_On& _Other) = delete;
	StreetLight_On(StreetLight_On&& _Other) noexcept = delete;
	StreetLight_On& operator=(const StreetLight_On& _Other) = delete;
	StreetLight_On& operator=(StreetLight_On&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	std::shared_ptr<class GameEngineLight> PointLight;

};

