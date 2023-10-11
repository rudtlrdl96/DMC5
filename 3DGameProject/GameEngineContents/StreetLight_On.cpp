#include "PrecompileHeader.h"
#include "StreetLight_On.h"

StreetLight_On::StreetLight_On()
{

}

StreetLight_On::~StreetLight_On()
{

}

void StreetLight_On::Start()
{
	FBXFileName = "sm0026_streetlight01.fbx";
	PointLight = GetLevel()->CreatePointLight({ 0.f, 720.f, 0.f }, ShadowTextureScale::S_512, 1000.f);
	PointLight->GetTransform()->SetParent(GetTransform());
	PointLight->SetLightPower(2.f);

	SetDebugRender();
	DebugRenderPivot->GetTransform()->SetLocalPosition(PointLight->GetTransform()->GetLocalPosition());

	StaticFieldMapObject::Start();
}

void StreetLight_On::Update(float _DeltaTime)
{
	DrawDebugRender(_DeltaTime);
	StaticFieldMapObject::Update(_DeltaTime);
}
