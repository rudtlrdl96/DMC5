#include "PrecompileHeader.h"
#include "illminary.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>

illminary::illminary()
{

}

illminary::~illminary()
{

}

void illminary::Start()
{
	FBXFileName = "LOD_1_Group_0_Sub_3__m02m_illminary01.fbx";

	StaticFieldMapObject::Start();

	FBXMesh->SetBaseColor({2.f,0.f,0.f,0.f});
}

void illminary::Update(float _DeltaTime)
{
}
