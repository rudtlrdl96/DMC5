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
	PointLight = GetLevel()->CreatePointLight({ 0.f, 40.f, 30.f} , { 100.f,100.f }, 1000.f);
	PointLight->GetTransform()->SetParent(GetTransform());
	PointLight->LightDataValue.LightPower = 2.f;

	StaticFieldMapObject::Start();
}
