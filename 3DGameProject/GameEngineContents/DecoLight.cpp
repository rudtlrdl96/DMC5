#include "PrecompileHeader.h"
#include "DecoLight.h"

DecoLight::DecoLight()
{

}

DecoLight::~DecoLight()
{

}

void DecoLight::Start()
{
	FBXFileName = "sm0183_decolight00.fbx";
	SpotLight = GetLevel()->CreateSpotLight({ 0.f, 0.f, 0.f }, ShadowTextureScale::S_512, 1000.f, 45.f);
	SpotLight->GetTransform()->SetLocalRotation({90,0,0});
	SpotLight->GetTransform()->SetParent(GetTransform());
	SpotLight->SetLightPower(2.f);

	SetDebugRender();
	DebugRenderPivot->GetTransform()->SetLocalPosition(SpotLight->GetTransform()->GetLocalPosition());

	StaticFieldMapObject::Start();
}

void DecoLight::Update(float _DeltaTime)
{
	DrawDebugRender(_DeltaTime);
	StaticFieldMapObject::Update(_DeltaTime);
}
