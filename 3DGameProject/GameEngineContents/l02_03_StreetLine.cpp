#include "PrecompileHeader.h"
#include "l02_03_StreetLine.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>

l02_03_StreetLine::l02_03_StreetLine()
{

}

l02_03_StreetLine::~l02_03_StreetLine()
{

}

void l02_03_StreetLine::Start()
{
	FBXFileName = "l02_03StreetLine.fbx";
	StaticFieldMapObject::Start();
	FBXMesh->ShadowOff();
}

void l02_03_StreetLine::Update(float _DeltaTime)
{
}
