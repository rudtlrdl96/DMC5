#include "PrecompileHeader.h"
#include "StreetLight_On.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>

StreetLight_On::StreetLight_On()
{

}

StreetLight_On::~StreetLight_On()
{

}

void StreetLight_On::Start()
{
	FBXFileName = "sm0026_streetlight01.fbx";

	PointLight = GetLevel()->CreatePointLight(GetTransform()->GetLocalPosition() + LightPosition, ShadowTextureScale::S_512, 1500.f);
	PointLight->GetTransform()->SetParent(GetTransform());
	PointLight->SetLightPower(2.f);
	PointLight->SetLightColor({1.f,0.7f,0.7f});

	StaticFieldMapObject::Start();

	LightValue = FBXMesh->GetRenderBaseValueRef();
	LightValue.BaseColor = { 10.0f, 0, 0, 0 };
	FBXMesh->GetRenderUnit(2, 0)->ShaderResHelper.SetConstantBufferLink("RenderBaseValue", LightValue);
}

void StreetLight_On::Update(float _DeltaTime)
{
	StaticFieldMapObject::Update(_DeltaTime);
}
