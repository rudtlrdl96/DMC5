#include "PrecompileHeader.h"
#include "TestObject.h"

#include <GameEngineCore/GameEngineFBXRenderer.h>
//#include <GameEngineCore/PhysXBoxGeometryComponent.h>
#include <GameEngineCore/PhysXDynamicActorComponent.h>

#include "TestLevel.h"

TestObject::TestObject() 
{
}

TestObject::~TestObject() 
{
}


void TestObject::Start()
{
	if (false == GameEngineInput::IsKey("MoveLeft"))
	{
		GameEngineInput::CreateKey("MoveLeft", 'A');
		GameEngineInput::CreateKey("MoveRight", 'D');
		GameEngineInput::CreateKey("MoveUp", 'Q');
		GameEngineInput::CreateKey("MoveDown", 'E');
		GameEngineInput::CreateKey("MoveForward", 'W');
		GameEngineInput::CreateKey("MoveBack", 'S');
		
	}
	if (false == GameEngineInput::IsKey("Jump"))
	{
		GameEngineInput::CreateKey("Jump", VK_SPACE);
	}

	GetTransform()->AddWorldPosition(float4{ 0, 100.0f, 0 });

	std::shared_ptr<GameEngineFBXRenderer> Renderer = CreateComponent<GameEngineFBXRenderer>();
	//Renderer->SetFBXMesh("House1.FBX", "MeshTexture");

	//Component = CreateComponent<PhysXDynamicActorComponent>();

	//float4 RenderMeshScale = Renderer->GetFBXMesh()->GetRenderUnit(0)->BoundScaleBox;
	//physx::PxVec3 GeoMetryScale = { RenderMeshScale.x, RenderMeshScale.y, RenderMeshScale.z };

	//Component->SetPhysxMaterial(0.0f, 0.0f, 1.0f);
	//Component->CreatePhysXActors(TestLevel::TestLevelPtr->GetLevelScene(), TestLevel::TestLevelPtr->GetLevelPhysics(), GeoMetryScale * 0.1f);
	//Component->TurnOffGravity();

	std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
		{
			"Character", "Enemy", "em0100", "mesh"
		}, "em0100.FBX");

	GameEngineFBXMesh::Load(Path);

	Renderer->SetFBXMesh("em0100.fbx", "NoneAlphaMesh");

	Renderer->GetTransform()->SetLocalScale({ 0.1f , 0.1f , 0.1f });

	float4 RenderScale = Renderer->GetFBXMesh()->GetRenderUnit(0)->BoundScaleBox;
	physx::PxVec3 VecSclae = { RenderScale.x, RenderScale.y, RenderScale.z };

	Component = CreateComponent<PhysXDynamicActorComponent>();
	Component->SetPhysxMaterial(0.0f, 0.0f, 0.0f);
	Component->CreatePhysXActors(TestLevel::TestLevelPtr->GetLevelScene(), TestLevel::TestLevelPtr->GetLevelPhysics(), VecSclae);
	//Component->TurnOffGravity();
}

void TestObject::Update(float _DeltaTime)
{
	//NetControllType Type = GetControllType();

	NetControllType Type = NetControllType::UserControll;

	switch (Type)
	{
	case NetControllType::UserControll:
		UserUpdate(_DeltaTime);
		break;
	case NetControllType::NetControll:
		ServerUpdate(_DeltaTime);
		break;
	default:
		break;
	}
}

void TestObject::UserUpdate(float _DeltaTime)
{
	float Speed = 30.0f;
	physx::PxTransform PhyTF = Component->GetDynamic()->getGlobalPose();
	float4 MoveDir = float4::ZERO;
	float Dir = Speed * _DeltaTime;

	if (true == GameEngineInput::IsPress("MoveLeft"))
	{
		PhyTF.p.x -= Dir;
		MoveDir.x -= Dir;

		Component->GetDynamic()->setGlobalPose(PhyTF);
		GetLevel()->GetCamera(0)->GetTransform()->AddWorldPosition(MoveDir);
	}
	if (true == GameEngineInput::IsPress("MoveRight"))
	{
		PhyTF.p.x += Dir;
		MoveDir.x += Dir;

		Component->GetDynamic()->setGlobalPose(PhyTF);
		GetLevel()->GetCamera(0)->GetTransform()->AddWorldPosition(MoveDir);
	}
	if (true == GameEngineInput::IsPress("MoveUp"))
	{
		PhyTF.p.y += Dir;
		MoveDir.y += Dir;

		Component->GetDynamic()->setGlobalPose(PhyTF);
		GetLevel()->GetCamera(0)->GetTransform()->AddWorldPosition(MoveDir);
	}
	if (true == GameEngineInput::IsPress("MoveDown"))
	{
		PhyTF.p.y -= Dir;
		MoveDir.y -= Dir;

		Component->GetDynamic()->setGlobalPose(PhyTF);
		GetLevel()->GetCamera(0)->GetTransform()->AddWorldPosition(MoveDir);
	}
	if (true == GameEngineInput::IsPress("MoveForward"))
	{
		PhyTF.p.z += Dir;
		MoveDir.z += Dir;

		Component->GetDynamic()->setGlobalPose(PhyTF);
		GetLevel()->GetCamera(0)->GetTransform()->AddWorldPosition(MoveDir);
	}
	if (true == GameEngineInput::IsPress("MoveBack"))
	{
		PhyTF.p.z -= Dir;
		MoveDir.z -= Dir;

		Component->GetDynamic()->setGlobalPose(PhyTF);
		GetLevel()->GetCamera(0)->GetTransform()->AddWorldPosition(MoveDir);
	}

	if (true == GameEngineInput::IsDown("Jump"))
	{
		PhyTF.p.y += 10.0f;
		MoveDir.y += 10.0f;

		Component->GetDynamic()->setGlobalPose(PhyTF);
		GetLevel()->GetCamera(0)->GetTransform()->AddWorldPosition(MoveDir);
	}

	float4 ASDF = Component->GetTransform()->GetWorldPosition();
	float4 FDSA = Component->GetTransform()->GetLocalPosition();
	float4 REWQ = Component->GetTransform()->GetWorldScale();
	float4 QWER = Component->GetTransform()->GetLocalScale();
}

void TestObject::ServerUpdate(float _DeltaTime)
{

}