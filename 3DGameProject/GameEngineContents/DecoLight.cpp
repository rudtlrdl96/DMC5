#include "PrecompileHeader.h"
#include "DecoLight.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>

DecoLight::DecoLight()
{

}

DecoLight::~DecoLight()
{

}

void DecoLight::Start()
{
	FBXFileName = "sm0183_decolight00.fbx";
	SpotLight = GetLevel()->CreateSpotLight(GetTransform()->GetLocalPosition() + LightPosition, ShadowTextureScale::S_1024, 3000.f, 120.f);
	SpotLight->GetTransform()->SetLocalRotation({-90,0,0});
	SpotLight->GetTransform()->SetParent(GetTransform());
	SpotLight->SetLightPower(3.f);
	SpotLight->DynamicShadowOn();
	SpotLight->IsDebugDraw = true;

	StaticFieldMapObject::Start();
}

void DecoLight::Update(float _DeltaTime)
{
	StaticFieldMapObject::Update(_DeltaTime);
}
