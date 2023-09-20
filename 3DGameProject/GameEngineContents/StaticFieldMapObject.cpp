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

void StaticFieldMapObject::Start()
{
	SetRenderer("FBX");
}

void StaticFieldMapObject::Update(float _DeltaTime)
{
	if (PhysXBox == nullptr)
	{
		SetPhysX();
	}
	//MapObj_SetFBXMesh(FBXFileName, "FBX");
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
}

void StaticFieldMapObject::SetPhysX()
{
	if (FBXMesh == nullptr)
	{
		MsgAssert("StaticFieldMapObject : FBXMesh�� nullptr�̾ PhysXBox�� ������ �� �����ϴ�")
	}

	PhysXBox = CreateComponent<PhysXBoxComponent>();
	const float4& MeshScale = FBXMesh->GetMeshScale();
	physx::PxVec3 PxScale = { MeshScale.x, MeshScale.y, MeshScale.z };
	PhysXBox->SetObstacleObject();
	PhysXBox->CreatePhysXActors(PxScale);
}
