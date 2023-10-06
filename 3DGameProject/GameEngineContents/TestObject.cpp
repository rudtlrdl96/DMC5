#include "PrecompileHeader.h"
#include "TestObject.h"

//#include <GameEngineCore/PhysXBoxComponent.h>
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/PhysXCapsuleComponent.h>
#include <GameEngineCore/GameEngineFontRenderer.h>
#include <GameEngineCore/PhysXTriangleComponent.h>

#include "TestLevel.h"

TestObject::TestObject() 
{
}

TestObject::~TestObject() 
{
}

void TestObject::Start()
{
	if (false == GameEngineInput::IsKey("MonTest"))
	{
		GameEngineInput::CreateKey("MonTest", 'M');
	}

	if (nullptr == GameEngineFBXMesh::Find("em0100.FBX"))
	{
		std::string Path = GameEnginePath::GetFileFullPath
		(
			"ContentResources",
			{
				"Character", "Enemy", "Empusa", "mesh"
			},
			"em0100.FBX"
		);
		GameEngineFBXMesh::Load(Path);
	}

	Renderer = CreateComponent<GameEngineFBXRenderer>();
	Renderer->SetFBXMesh("em0100.fbx", "AniFBX_Low");
	//Renderer->GetTransform()->SetLocalScale({ 0.1f , 0.1f, 0.1f });
	Renderer->LightOff();

	float4 RenderScale = Renderer->GetFBXMesh()->GetRenderUnit(0)->BoundScaleBox;
	float4 MeshScale = Renderer->GetMeshScale();

	physx::PxVec3 VecSclae = { RenderScale.x, RenderScale.y, RenderScale.z };

	Component = CreateComponent<PhysXCapsuleComponent>();
	Component->SetPhysxMaterial(0, 0, 0);
	Component->CreatePhysXActors({ 90, 60, 90 });
	Component->GetDynamic()->setMass(10.0f);
	Component->GetDynamic()->setLinearDamping(10.0f);

	std::shared_ptr<GameEngineFBXRenderer> Renderer = CreateComponent<GameEngineFBXRenderer>();

	Component->GetDynamic()->setMass(80.f);
	Component->GetDynamic()->setGlobalPose({0.0f, 200.0f, 0.0f});
}

int TestCount = 0;

void TestObject::Update(float _DeltaTime)
{
	if (true == GameEngineInput::IsDown("MonTest"))
	{
		Disable = !Disable;
		TestCount = 1;
	}

	if (1 == TestCount)
	{
		TestCount = 0;
		if (true == Disable)
		{
			Component->Off();
		}
		else
		{
			Component->On();
		}
	}
}
