#include "PrecompileHeader.h"
#include "WallLight_Off.h"

WallLight_Off::WallLight_Off()
{

}

WallLight_Off::~WallLight_Off()
{

}

void WallLight_Off::Start()
{
	FBXFileName = "sm0003_wall_light.fbx";
	StaticFieldMapObject::Start();
}
