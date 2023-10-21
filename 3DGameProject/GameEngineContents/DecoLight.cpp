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
	SpotLight = GetLevel()->CreateSpotLight(GetTransform()->GetLocalPosition() + LightPosition, ShadowTextureScale::S_512, 2000.f, 90.f);
	SpotLight->GetTransform()->SetLocalRotation({-90,0,0});
	SpotLight->GetTransform()->SetParent(GetTransform());
	SpotLight->SetLightPower(40.f);

	StaticFieldMapObject::Start();
}

void DecoLight::Update(float _DeltaTime)
{
	StaticFieldMapObject::Update(_DeltaTime);
}
