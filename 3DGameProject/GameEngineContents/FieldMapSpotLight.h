#pragma once
#include "StaticFieldMapObject.h"

class FieldMapSpotLight : public StaticFieldMapObject
{
public:
	// construtor destructor
	FieldMapSpotLight();
	~FieldMapSpotLight();

	// delete Function
	FieldMapSpotLight(const FieldMapSpotLight& _Other) = delete;
	FieldMapSpotLight(FieldMapSpotLight&& _Other) noexcept = delete;
	FieldMapSpotLight& operator=(const FieldMapSpotLight& _Other) = delete;
	FieldMapSpotLight& operator=(FieldMapSpotLight&& _Other) noexcept = delete;

protected:
	void Start() override;

private:
	std::shared_ptr<class GameEngineLight> SpotLight = nullptr;

};

