#include "PrecompileHeader.h"
#include "FieldMapSpotLight.h"

FieldMapSpotLight::FieldMapSpotLight()
{

}

FieldMapSpotLight::~FieldMapSpotLight()
{

}

void FieldMapSpotLight::Start()
{
	SpotLight = GetLevel()->CreateSpotLight(GetTransform()->GetLocalPosition(), ShadowTextureScale::S_1024, 3000.f, 120.f);
	SpotLight->GetTransform()->SetLocalRotation({ 0,0,0 });
	SpotLight->GetTransform()->SetParent(GetTransform());
	SpotLight->SetLightPower(3.f);
	SpotLight->DynamicShadowOn();
	SpotLight->IsDebugDraw = true;
}
