#include "PrecompileHeader.h"
#include "StaticFieldMapObject.h"

#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/PhysXBoxComponent.h>

StaticFieldMapObject::StaticFieldMapObject()
{

}

StaticFieldMapObject::~StaticFieldMapObject()
{

}

void StaticFieldMapObject::Update(float _DeltaTime)
{
	if (FBXMesh == nullptr)
	{
		MapObj_FBXMesh(FBXFileName, "FBX");
	}
}

void StaticFieldMapObject::MapObj_FBXMesh(const std::string_view& _Name, const std::string_view& _Mat)
{
	FBXMesh = CreateComponent<GameEngineFBXRenderer>();

	if (GameEngineOption::GetOption("Shader") == GameEngineOptionValue::Low)
	{
		FBXMesh->SetFBXMesh(_Name.data(), _Mat.data() + std::string("_Low"));
	}
	else
	{
		FBXMesh->SetFBXMesh(_Name.data(), _Mat.data());
	}

	PhysXBox = CreateComponent<PhysXBoxComponent>();
	physx::PxVec3 PxScale = { FBXMesh->GetMeshScale().x, FBXMesh->GetMeshScale().y, FBXMesh->GetMeshScale().z };
	PhysXBox->SetObstacleObject();
	PhysXBox->CreatePhysXActors(PxScale);
}