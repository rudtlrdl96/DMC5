#include "PrecompileHeader.h"
#include "FieldMapPointLight.h"

FieldMapPointLight::FieldMapPointLight()
{

}

FieldMapPointLight::~FieldMapPointLight()
{

}

void FieldMapPointLight::Start()
{
	PointLight = GetLevel()->CreatePointLight(GetTransform()->GetLocalPosition(), ShadowTextureScale::S_1024, 2000.f);
	PointLight->GetTransform()->SetLocalRotation({ 0,0,0 });
	PointLight->GetTransform()->SetParent(GetTransform());
	PointLight->SetLightPower(2.f);
	PointLight->SetLightColor({ 0.95f, 0.85f, 0.6f });
	PointLight->DynamicShadowOn();
	PointLight->IsDebugDraw = true;
}

void FieldMapPointLight::Update(float _DeltaTime)
{
}
