#include "PrecompileHeader.h"
#include "TestObject.h"

#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/PhysXBoxGeometryComponent.h>

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

	std::shared_ptr<GameEngineFBXRenderer> Renderer = CreateComponent<GameEngineFBXRenderer>();
	Renderer->SetFBXMesh("House1.FBX", "MeshTexture");

	

	float4 RenderMeshScale = Renderer->GetFBXMesh()->GetRenderUnit(0)->BoundScaleBox;
	physx::PxVec3 GeoMetryScale = { RenderMeshScale.x, RenderMeshScale.y, RenderMeshScale.z };

	Renderer->GetTransform()->AddLocalPosition(float4{ 0, -RenderMeshScale.y / 2, 0 });

	Component = CreateComponent<PhysXBoxGeometryComponent>();
	Component->SetIsGravity(true);

	Component->SetRestitution(2.0f);
	//Component->SetPositionSetFromParentFlag(true);
	Component->CreatePhysXActors(TestLevel::TestLevelPtr->GetLevelScene(), TestLevel::TestLevelPtr->GetLevelPhysics(), GeoMetryScale);
	//Component->SetPositionSetFromParentFlag(true);
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
	float Speed = 200.0f;

	if (true == GameEngineInput::IsPress("MoveLeft"))
	{
		GetTransform()->AddLocalPosition(GetTransform()->GetWorldLeftVector() * Speed * _DeltaTime);
	}
	if (true == GameEngineInput::IsPress("MoveRight"))
	{
		GetTransform()->AddLocalPosition(GetTransform()->GetWorldRightVector() * Speed * _DeltaTime);
	}
	if (true == GameEngineInput::IsPress("MoveUp"))
	{
		GetTransform()->AddLocalPosition(GetTransform()->GetWorldUpVector() * Speed * _DeltaTime);
	}
	if (true == GameEngineInput::IsPress("MoveDown"))
	{
		GetTransform()->AddLocalPosition(GetTransform()->GetWorldDownVector() * Speed * _DeltaTime);
	}
	if (true == GameEngineInput::IsPress("MoveForward"))
	{
		GetTransform()->AddLocalPosition(GetTransform()->GetWorldForwardVector() * Speed * _DeltaTime);
	}
	if (true == GameEngineInput::IsPress("MoveBack"))
	{
		GetTransform()->AddLocalPosition(GetTransform()->GetWorldBackVector() * Speed * _DeltaTime);
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