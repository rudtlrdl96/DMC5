#pragma once
#include "StaticFieldMapObject.h"

class ElectronicBillboard : public StaticFieldMapObject
{
public:
	// construtor destructor
	ElectronicBillboard();
	~ElectronicBillboard();

	// delete Function
	ElectronicBillboard(const ElectronicBillboard& _Other) = delete;
	ElectronicBillboard(ElectronicBillboard&& _Other) noexcept = delete;
	ElectronicBillboard& operator=(const ElectronicBillboard& _Other) = delete;
	ElectronicBillboard& operator=(ElectronicBillboard&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	std::shared_ptr<class GameEngineLight> PointLight0 = nullptr;
	std::shared_ptr<class GameEngineLight> PointLight1 = nullptr;
	float4 LightPosition0 = {-600.f, 1200.f, -200.f};
	float4 LightPosition1 = { 530.f, 1200.f, -420.f };
	class RenderBaseValue LightValue;
};
