#include "PrecompileHeader.h"
#include "DynamicResourcesLoadTest.h"

DynamicResourcesLoadTest::DynamicResourcesLoadTest() 
{
}

DynamicResourcesLoadTest::~DynamicResourcesLoadTest() 
{
}



void DynamicResourcesLoadTest::Start()
{
	GetMainCamera()->SetProjectionType(CameraType::Perspective);

	//Loader.AddGroupTexture(0, "T", "");

}

void DynamicResourcesLoadTest::Update(float _DeltaTime)
{

}

void DynamicResourcesLoadTest::LevelChangeStart()
{

}

void DynamicResourcesLoadTest::LevelChangeEnd()
{

}