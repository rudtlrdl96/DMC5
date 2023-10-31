#pragma once
#include "StaticFieldMapObject.h"

class FieldMapPointLight : public StaticFieldMapObject
{
public:
	// construtor destructor
	FieldMapPointLight();
	~FieldMapPointLight();

	// delete Function
	FieldMapPointLight(const FieldMapPointLight& _Other) = delete;
	FieldMapPointLight(FieldMapPointLight&& _Other) noexcept = delete;
	FieldMapPointLight& operator=(const FieldMapPointLight& _Other) = delete;
	FieldMapPointLight& operator=(FieldMapPointLight&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	std::shared_ptr<class GameEngineLight> PointLight = nullptr;

};

