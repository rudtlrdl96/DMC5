#include "PrecompileHeader.h"
#include "StaticFieldMapObject.h"

#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/PhysXBoxComponent.h>
#include <GameEngineCore/PhysXTriangleComponent.h>

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
	if (PhysX == nullptr && !GetLevel()->DynamicThis<StageBaseLevel>()->IsEditLevel)
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
	FBXMesh->SetStatic();
	FBXMesh->ShadowOn();
}

void StaticFieldMapObject::SetPhysX()
{
	if (FBXMesh == nullptr)
	{
		MsgAssert("StaticFieldMapObject : FBXMesh가 nullptr이어서 PhysXBox를 세팅할 수 없습니다")
	}

	PhysX = CreateComponent<PhysXTriangleComponent>();
	PhysX->SetPhysxMaterial(1.0f, 1.0f, 0.05f);
	//const float4& AcScale = GetTransform()->GetLocalScale();
	//const float4& MeshScale = FBXMesh->GetMeshScale();
	//physx::PxVec3 PxScale = { MeshScale.x * AcScale.x, MeshScale.y * AcScale.y, MeshScale.z * AcScale.z };
	PhysX->SetObstacleObject();
	PhysX->SetPositionSetFromParentFlag(true);
	PhysX->CreatePhysXActors(FBXFileName);
}
