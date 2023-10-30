#include "PrecompileHeader.h"
#include "StreetLight_On.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include "StageBaseLevel.h"

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

	if (!GetLevel()->DynamicThis<StageBaseLevel>()->IsEditLevel)
	{
		PointLight = GetLevel()->CreatePointLight(GetTransform()->GetLocalPosition() + LightPosition, ShadowTextureScale::S_1024, 1500.f);
		PointLight->GetTransform()->SetParent(GetTransform());
		PointLight->SetLightPower(2.f);
		PointLight->SetLightColor({ 0.95f, 0.85f, 0.6f });
		PointLight->DynamicShadowOn();
		PointLight->IsDebugDraw = true;
	}
	
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
