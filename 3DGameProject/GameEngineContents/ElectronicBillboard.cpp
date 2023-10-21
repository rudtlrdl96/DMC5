#include "PrecompileHeader.h"
#include "ElectronicBillboard.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>

ElectronicBillboard::ElectronicBillboard()
{

}

ElectronicBillboard::~ElectronicBillboard()
{

}

void ElectronicBillboard::Start()
{
	FBXFileName = "sm0071_electronicbillboard00.fbx";

	SetDebugRender();
	DebugRenderPivot->GetTransform()->SetLocalPosition(LightPosition0);
	PointLight0 = GetLevel()->CreatePointLight(GetTransform()->GetLocalPosition() + LightPosition0, ShadowTextureScale::S_512, 1500.f);
	PointLight0->GetTransform()->SetParent(GetTransform());
	PointLight0->SetLightPower(1.f);
	PointLight0->SetLightRange(1000.f);

	DebugRenderPivot->GetTransform()->SetLocalPosition(LightPosition1);
	PointLight1 = GetLevel()->CreatePointLight(GetTransform()->GetLocalPosition() + LightPosition1, ShadowTextureScale::S_512, 1500.f);
	PointLight1->GetTransform()->SetParent(GetTransform());
	PointLight1->SetLightPower(1.f);
	PointLight1->SetLightRange(1000.f);

	StaticFieldMapObject::Start();

	LightValue = FBXMesh->GetRenderBaseValueRef();
	
	LightValue.BaseColor = { 2.0f, 0, 0, 0 };

	FBXMesh->GetRenderUnit(0, 0)->ShaderResHelper.SetConstantBufferLink("RenderBaseValue", LightValue);
	FBXMesh->GetRenderUnit(3, 0)->ShaderResHelper.SetConstantBufferLink("RenderBaseValue", LightValue);
	FBXMesh->GetRenderUnit(4, 0)->ShaderResHelper.SetConstantBufferLink("RenderBaseValue", LightValue);
	FBXMesh->GetRenderUnit(5, 0)->ShaderResHelper.SetConstantBufferLink("RenderBaseValue", LightValue);
}

void ElectronicBillboard::Update(float _DeltaTime)
{
	DrawDebugRender(_DeltaTime);
}
