#include "PrecompileHeader.h"
#include "WallLight_On.h"
#include <GameEngineCore/GameEngineLight.h>
#include "StageBaseLevel.h"

WallLight_On::WallLight_On()
{

}

WallLight_On::~WallLight_On()
{

}

void WallLight_On::Start()
{
	FBXFileName = "sm0003_wall_light.fbx";

	SetDebugRender();
	DebugRenderPivot->GetTransform()->SetLocalPosition(LightPosition);
	PointLight = GetLevel()->CreatePointLight(GetTransform()->GetLocalPosition() + LightPosition, ShadowTextureScale::S_512, 1500.f);
	PointLight->GetTransform()->SetParent(GetTransform());
	PointLight->SetLightPower(1.f);

	StaticFieldMapObject::Start();
}

void WallLight_On::Update(float _DeltaTime)
{
	DrawDebugRender(_DeltaTime);
	StaticFieldMapObject::Update(_DeltaTime);
}
