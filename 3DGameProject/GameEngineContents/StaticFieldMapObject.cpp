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
	CullingExeption = false;
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
		MsgAssert("StaticFieldMapObject : �ҷ��� fbx������ �������� �ʾҽ��ϴ�.")
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
		MsgAssert("StaticFieldMapObject : FBXMesh�� nullptr�̾ PhysXBox�� ������ �� �����ϴ�")
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
