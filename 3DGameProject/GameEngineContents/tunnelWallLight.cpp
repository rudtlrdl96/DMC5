#include "PrecompileHeader.h"
#include "tunnelWallLight.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include "StageBaseLevel.h"

tunnelWallLight::tunnelWallLight()
{

}

tunnelWallLight::~tunnelWallLight()
{

}

void tunnelWallLight::Start()
{
	FBXFileName = "sm0536_toplight.fbx";

	PointLight = GetLevel()->CreatePointLight(GetTransform()->GetLocalPosition() + LightPosition, ShadowTextureScale::S_1024, 1500.f);
	PointLight->GetTransform()->SetParent(GetTransform());
	PointLight->GetTransform()->SetLocalPosition({ 90.f,0,0 });
	PointLight->SetLightPower(1.f);
	PointLight->SetLightColor({ 0.95f, 0.85f, 0.6f });
	PointLight->DynamicShadowOn();
	PointLight->ShadowOn();
	PointLight->IsDebugDraw = true;

	StaticFieldMapObject::Start();

	LightValue = FBXMesh->GetRenderBaseValueRef();
	LightValue.BaseColor = { 10.0f, 0, 0, 0 };
	FBXMesh->GetRenderUnit(0, 0)->ShaderResHelper.SetConstantBufferLink("RenderBaseValue", LightValue);
	FBXMesh->ShadowOff();
}

void tunnelWallLight::Update(float _DeltaTime)
{
}
