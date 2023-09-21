#include "PrecompileHeader.h"
#include "StaticFieldMapObject.h"

#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/PhysXBoxComponent.h>

#include "StageBaseLevel.h"

StaticFieldMapObject::StaticFieldMapObject()
{

}

StaticFieldMapObject::~StaticFieldMapObject()
{

}

void StaticFieldMapObject::Start()
{
	SetRenderer("FBX");
}

void StaticFieldMapObject::Update(float _DeltaTime)
{
	if (PhysXBox == nullptr && !GetLevel()->DynamicThis<StageBaseLevel>()->IsEditLevel)
	{
		SetPhysX();
	}
}

void StaticFieldMapObject::SetRenderer(const std::string_view& _Mat)
{
	if (FBXFileName == "")
	{
		MsgAssert("StaticFieldMapObject : 불러올 fbx파일이 정해지지 않았습니다.")
	}

	FBXMesh = CreateComponent<GameEngineFBXRenderer>();

	if (GameEngineOption::GetOption("Shader") == GameEngineOptionValue::Low)
	{
		FBXMesh->SetFBXMesh(FBXFileName.data(), _Mat.data() + std::string("_Low"));
	}
	else
	{
		FBXMesh->SetFBXMesh(FBXFileName.data(), _Mat.data());
	}
}

void StaticFieldMapObject::SetPhysX()
{
	if (FBXMesh == nullptr)
	{
		MsgAssert("StaticFieldMapObject : FBXMesh가 nullptr이어서 PhysXBox를 세팅할 수 없습니다")
	}

	PhysXBox = CreateComponent<PhysXBoxComponent>();
	const float4& MeshScale = FBXMesh->GetMeshScale();
	physx::PxVec3 PxScale = { MeshScale.x, MeshScale.y, MeshScale.z };
	PhysXBox->SetObstacleObject();
	PhysXBox->CreatePhysXActors(PxScale);
}
