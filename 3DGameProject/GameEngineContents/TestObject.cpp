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
		GameEngineInput::CreateKey("Jump", VK_SPACE);
	}

	std::shared_ptr<GameEngineFBXRenderer> Renderer = CreateComponent<GameEngineFBXRenderer>();
	Renderer->SetFBXMesh("House1.FBX", "MeshTexture");

	

	//float4 RenderMeshScale = Renderer->GetFBXMesh()->GetRenderUnit(0)->BoundScaleBox;
	//physx::PxVec3 GeoMetryScale = { RenderMeshScale.x, RenderMeshScale.y, RenderMeshScale.z };

	//Renderer->GetTransform()->AddLocalPosition(float4{ 0, -RenderMeshScale.y / 2, 0 });

	Component = CreateComponent<PhysXDynamicActorComponent>();
	//Component->SetIsGravity(true);

	////Component->SetPositionSetFromParentFlag(true);
	//Component->CreatePhysXActors(TestLevel::TestLevelPtr->GetLevelScene(), TestLevel::TestLevelPtr->GetLevelPhysics(), GeoMetryScale);
	////Component->SetPositionSetFromParentFlag(true);

	float4 RenderMeshScale = Renderer->GetFBXMesh()->GetRenderUnit(0)->BoundScaleBox;
	physx::PxVec3 GeoMetryScale = { RenderMeshScale.x, RenderMeshScale.y, RenderMeshScale.z };

	Component->SetPhysxMaterial(0.0f, 0.0f, 1.0f);
	Component->CreatePhysXActors(TestLevel::TestLevelPtr->GetLevelScene(), TestLevel::TestLevelPtr->GetLevelPhysics(), GeoMetryScale * 0.1f);
	//Component->TurnOffGravity();
}

void TestObject::Update(float _DeltaTime)
{
	NetControllType Type = GetControllType();
	switch (Type)
	{
	case NetControllType::None:
		UserUpdate(_DeltaTime);
		break;
	case NetControllType::UserControll:
		UserUpdate(_DeltaTime);
		break;
	case NetControllType::ServerControll:
		ServerUpdate(_DeltaTime);
		break;
	default:
		break;
	}
}

void TestObject::UserUpdate(float _DeltaTime)
{
	float Speed = 30.0f;

	if (true == GameEngineInput::IsPress("MoveLeft"))
	{
		float4 WorldPos = GetTransform()->GetWorldPosition();
		physx::PxTransform PhyTF = Component->GetDynamic()->getGlobalPose();
		physx::PxVec3 MovePos = Component->GetDynamic()->getGlobalPose().p;

		float Dir = Speed * _DeltaTime;

		PhyTF.p.x -= Dir;

		Component->GetDynamic()->setGlobalPose(PhyTF);

		//GetTransform()->AddLocalPosition(GetTransform()->GetWorldLeftVector() * Speed * _DeltaTime);
	}
	if (true == GameEngineInput::IsPress("MoveRight"))
	{
		float4 WorldPos = GetTransform()->GetWorldPosition();
		physx::PxTransform PhyTF = Component->GetDynamic()->getGlobalPose();
		physx::PxVec3 MovePos = Component->GetDynamic()->getGlobalPose().p;

		float Dir = Speed * _DeltaTime;

		PhyTF.p.x += Dir;

		Component->GetDynamic()->setGlobalPose(PhyTF);

		//GetTransform()->AddLocalPosition(GetTransform()->GetWorldRightVector() * Speed * _DeltaTime);
	}
	if (true == GameEngineInput::IsPress("MoveUp"))
	{
		float4 WorldPos = GetTransform()->GetWorldPosition();
		physx::PxTransform PhyTF = Component->GetDynamic()->getGlobalPose();
		physx::PxVec3 MovePos = Component->GetDynamic()->getGlobalPose().p;

		float Dir = Speed * _DeltaTime;

		PhyTF.p.y += Dir;

		Component->GetDynamic()->setGlobalPose(PhyTF);

		//GetTransform()->AddLocalPosition(GetTransform()->GetWorldUpVector() * Speed * _DeltaTime);
	}
	if (true == GameEngineInput::IsPress("MoveDown"))
	{
		float4 WorldPos = GetTransform()->GetWorldPosition();
		physx::PxTransform PhyTF = Component->GetDynamic()->getGlobalPose();
		physx::PxVec3 MovePos = Component->GetDynamic()->getGlobalPose().p;

		float Dir = Speed * _DeltaTime;

		PhyTF.p.y -= Dir;

		Component->GetDynamic()->setGlobalPose(PhyTF);

		//GetTransform()->AddLocalPosition(GetTransform()->GetWorldDownVector() * Speed * _DeltaTime);
	}
	if (true == GameEngineInput::IsPress("MoveForward"))
	{
		float4 WorldPos = GetTransform()->GetWorldPosition();
		physx::PxTransform PhyTF = Component->GetDynamic()->getGlobalPose();
		physx::PxVec3 MovePos = Component->GetDynamic()->getGlobalPose().p;

		float Dir = Speed * _DeltaTime;

		PhyTF.p.z += Dir;

		Component->GetDynamic()->setGlobalPose(PhyTF);

		//GetTransform()->AddLocalPosition(GetTransform()->GetWorldForwardVector() * Speed * _DeltaTime);
	}
	if (true == GameEngineInput::IsPress("MoveBack"))
	{
		float4 WorldPos = GetTransform()->GetWorldPosition();
		physx::PxTransform PhyTF = Component->GetDynamic()->getGlobalPose();
		physx::PxVec3 MovePos = Component->GetDynamic()->getGlobalPose().p;

		float Dir = Speed * _DeltaTime;

		PhyTF.p.z -= Dir;

		Component->GetDynamic()->setGlobalPose(PhyTF);

		//GetTransform()->AddLocalPosition(GetTransform()->GetWorldBackVector() * Speed * _DeltaTime);
	}

	if (true == GameEngineInput::IsDown("Jump"))
	{
		float4 WorldPos = GetTransform()->GetWorldPosition();
		physx::PxTransform PhyTF = Component->GetDynamic()->getGlobalPose();
		physx::PxVec3 MovePos = Component->GetDynamic()->getGlobalPose().p;

		//float Dir = Speed * _DeltaTime;

		PhyTF.p.y += 10.0f;

		Component->GetDynamic()->setGlobalPose(PhyTF);
	}

	float4 ASDF = Component->GetTransform()->GetWorldPosition();
	float4 FDSA = Component->GetTransform()->GetLocalPosition();
	float4 REWQ = Component->GetTransform()->GetWorldScale();
	float4 QWER = Component->GetTransform()->GetLocalScale();

	int a = 0;

}

void TestObject::ServerUpdate(float _DeltaTime)
{

}