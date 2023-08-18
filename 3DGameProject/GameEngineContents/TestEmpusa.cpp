#include "PrecompileHeader.h"
#include "TestEmpusa.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>

#include <GameEngineCore/PhysXDynamicActorComponent.h>

#include "TestLevel.h"

TestEmpusa::TestEmpusa()
{
}

TestEmpusa::~TestEmpusa()
{
}

void TestEmpusa::EnemyMeshLoad()
{
	std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
		{
			"Character", "Enemy", "em0100", "mesh"
		}, "em0100.FBX");

	GameEngineFBXMesh::Load(Path);

	EnemyRenderer->SetFBXMesh("em0100.fbx", "NoneAlphaMesh");

	float4 RenderScale = EnemyRenderer->GetFBXMesh()->GetRenderUnit(0)->BoundScaleBox;
	physx::PxVec3 VecSclae = { RenderScale.x, RenderScale.y, RenderScale.z };

	Component->SetPhysxMaterial(0.0f, 0.0f, 0.0f);
	Component->CreatePhysXActors(TestLevel::TestLevelPtr->GetLevelScene(), TestLevel::TestLevelPtr->GetLevelPhysics(), VecSclae);
	Component->TurnOffGravity();

}

void TestEmpusa::EnemyTextureLoad()
{
}

void TestEmpusa::EnemyAnimationLoad()
{
}

void TestEmpusa::EnemyCreateFSM()
{
}
