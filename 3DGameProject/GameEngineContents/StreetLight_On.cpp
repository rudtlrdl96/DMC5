#include "PrecompileHeader.h"
#include "StreetLight_On.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>

StreetLight_On::StreetLight_On()
{
	SetName("StreetLight_On");
}

StreetLight_On::~StreetLight_On()
{

}

void StreetLight_On::Start()
{
	FBXFileName = "sm0026_streetlight01.fbx";

	PointLight = GetLevel()->CreatePointLight(GetTransform()->GetLocalPosition() + LightPosition, ShadowTextureScale::S_1024, 1500.f);
	PointLight->GetTransform()->SetParent(GetTransform());
	PointLight->SetLightPower(2.f);
	PointLight->SetLightColor({0.95f, 0.85f, 0.6f});
	PointLight->IsDebugDraw = true;

#ifdef _DEBUG

#else
	//PointLight->DynamicShadowOn();
#endif // _DEBUG

	StaticFieldMapObject::Start();

	LightValue = FBXMesh->GetRenderBaseValueRef();
	LightValue.BaseColor = { 10.0f, 0, 0, 0 };
	FBXMesh->GetRenderUnit(2, 0)->ShaderResHelper.SetConstantBufferLink("RenderBaseValue", LightValue);
	FBXMesh->ShadowOff();
}

void StreetLight_On::Update(float _DeltaTime)
{
	StaticFieldMapObject::Update(_DeltaTime);
}
