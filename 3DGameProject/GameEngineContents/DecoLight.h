#pragma once
#include "StaticFieldMapObject.h"
class DecoLight : public StaticFieldMapObject
{
public:
	// construtor destructor
	DecoLight();
	~DecoLight();

	// delete Function
	DecoLight(const DecoLight& _Other) = delete;
	DecoLight(DecoLight&& _Other) noexcept = delete;
	DecoLight& operator=(const DecoLight& _Other) = delete;
	DecoLight& operator=(DecoLight&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	std::shared_ptr<class GameEngineLight> SpotLight = nullptr;
	float4 LightPosition = { 0.f, 20.f, 0.f };
};

