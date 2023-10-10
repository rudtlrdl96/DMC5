#include "PrecompileHeader.h"
#include "WallLight_On.h"
#include <GameEngineCore/GameEngineLight.h>

WallLight_On::WallLight_On()
{

}

WallLight_On::~WallLight_On()
{

}

void WallLight_On::Start()
{
	FBXFileName = "sm0003_wall_light.fbx";
	PointLight = GetLevel()->CreatePointLight({ 0.f, 40.f, 30.f} , ShadowTextureScale::S_512, 1000.f);
	PointLight->GetTransform()->SetParent(GetTransform());
	PointLight->SetLightPower(2.f);

	SetDebugRender();

	StaticFieldMapObject::Start();
}

void WallLight_On::Update(float _DeltaTime)
{
	DebugRenderPivot->GetTransform()->SetLocalPosition(PointLight->GetTransform()->GetLocalPosition());
	DrawDebugRender(_DeltaTime);
}
