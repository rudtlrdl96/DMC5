#include "PrecompileHeader.h"
#include "StreetLight_Off.h"

StreetLight_Off::StreetLight_Off()
{

}

StreetLight_Off::~StreetLight_Off()
{

}

void StreetLight_Off::Start()
{
	FBXFileName = "sm0026_streetlight01.fbx";

	StaticFieldMapObject::Start();
}
