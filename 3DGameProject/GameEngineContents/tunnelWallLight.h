#pragma once
#include "StaticFieldMapObject.h"

class tunnelWallLight : public StaticFieldMapObject
{
public:
	// construtor destructor
	tunnelWallLight();
	~tunnelWallLight();

	// delete Function
	tunnelWallLight(const tunnelWallLight& _Other) = delete;
	tunnelWallLight(tunnelWallLight&& _Other) noexcept = delete;
	tunnelWallLight& operator=(const tunnelWallLight& _Other) = delete;
	tunnelWallLight& operator=(tunnelWallLight&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	std::shared_ptr<class GameEngineLight> PointLight = nullptr;
	float4 LightPosition = { 0.f, -50.f, 0.f };
	class RenderBaseValue LightValue;

};

