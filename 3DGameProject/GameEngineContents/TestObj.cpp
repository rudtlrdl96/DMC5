#include "PrecompileHeader.h"
#include "TestObj.h"

#include <GameEngineCore/GameEngineFBXRenderer.h>

TestObj::TestObj()
{

}

TestObj::~TestObj()
{
	
}

void TestObj::Start()
{
	FBXFileName = "sealhellmini.fbx";
	StaticFieldMapObject::Start();
}
