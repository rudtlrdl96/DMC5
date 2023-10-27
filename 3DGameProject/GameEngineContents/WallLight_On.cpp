#include "PrecompileHeader.h"
#include "WallLight_On.h"
#include <GameEngineCore/GameEngineLight.h>
#include "StageBaseLevel.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>

WallLight_On::WallLight_On()
{

}

WallLight_On::~WallLight_On()
{

}

void WallLight_On::Start()
{
	FBXFileName = "sm0003_wall_light.fbx";

	PointLight = GetLevel()->CreatePointLight(GetTransform()->GetLocalPosition() + LightPosition, ShadowTextureScale::S_512, 1500.f);
	PointLight->GetTransform()->SetParent(GetTransform());
	PointLight->SetLightPower(1.f);
	PointLight->SetLightColor({ 0.95f, 0.85f, 0.6f });
	PointLight->DynamicShadowOn();

	StaticFieldMapObject::Start();

	LightValue = FBXMesh->GetRenderBaseValueRef();
	LightValue.BaseColor = { 10.0f, 0, 0, 0 };
	FBXMesh->GetRenderUnit(1, 0)->ShaderResHelper.SetConstantBufferLink("RenderBaseValue", LightValue);
	FBXMesh->GetRenderUnit(1, 0)->ShadowOff();
}

void WallLight_On::Update(float _DeltaTime)
{
	StaticFieldMapObject::Update(_DeltaTime);
}
