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
	if (false == GameEngineInput::IsKey("MoveLeft"))
	{
		GameEngineInput::CreateKey("MoveLeft", 'A');
		GameEngineInput::CreateKey("MoveRight", 'D');
		GameEngineInput::CreateKey("MoveUp", 'Q');
		GameEngineInput::CreateKey("MoveDown", 'E');
		GameEngineInput::CreateKey("MoveForward", 'W');
		GameEngineInput::CreateKey("MoveBack", 'S');
		GameEngineInput::CreateKey("RotateRight", 'T');
		GameEngineInput::CreateKey("RotateLeft", 'R');
		GameEngineInput::CreateKey("Select_01", 'Y');
		GameEngineInput::CreateKey("Select_02", 'U');
	}
	if (false == GameEngineInput::IsKey("Jump"))
	{
		GameEngineInput::CreateKey("Jump", VK_SPACE);
	}

	if (nullptr == GameEngineFBXMesh::Find("em0100.FBX"))
	{
		std::string Path = GameEnginePath::GetFileFullPath
		(
			"ContentResources",
			{
				"Character", "Enemy", "em0100", "mesh"
			},
			"em0100.FBX"
		);
		GameEngineFBXMesh::Load(Path);
	}

	Renderer = CreateComponent<GameEngineFBXRenderer>();
	Renderer->SetFBXMesh("em0100.fbx", "FBX");
	//Renderer->SetFBXMesh("Knight.fbx", "FBX");
	Renderer->GetTransform()->SetLocalScale({ 0.1f , 0.1f, 0.1f });
	Renderer->LightOff();

	float4 RenderScale = Renderer->GetFBXMesh()->GetRenderUnit(0)->BoundScaleBox;
	float4 MeshScale = Renderer->GetMeshScale();

	physx::PxVec3 VecSclae = { RenderScale.x, RenderScale.y, RenderScale.z };
	//physx::PxVec3 VecSclae = { 3.5f, 3.5f, 3.5f };

	Component = CreateComponent<PhysXCapsuleComponent>();
	Component->SetPhysxMaterial(0.0f, 0.0f, 0.0f);
	Component->CreatePhysXActors(VecSclae * 0.1f);

	//TriCom = CreateComponent<PhysXTriangleComponent>();
	//TriCom->SetPhysxMaterial(0.0f, 0.0f, 0.0f);
	//TriCom->CreatePhysXActors("Knight.fbx", GetLevel()->GetLevelScene(), GetLevel()->GetLevelPhysics(), GetLevel()->GetCooking(), false, VecSclae);

	std::shared_ptr<class GameEngineFontRenderer> FontRender = CreateComponent<GameEngineFontRenderer>();

	FontRender->GetTransform()->SetLocalPosition(GetTransform()->GetWorldPosition());
	FontRender->SetFont("DMC5Font");
	FontRender->SetScale(30.0f);
	FontRender->SetText("Say, fellas. thanks for\nsettling things down\naround here.");

	std::shared_ptr<GameEngineFBXRenderer> Renderer = CreateComponent<GameEngineFBXRenderer>();

	Component->GetDynamic()->setMass(10.f);
	Component->GetDynamic()->setGlobalPose({0.0f, 200.0f, 0.0f});
	//Component->GetDynamic()->setLinearDamping(physx::PxReal(1.f));
	//Component->GetDynamic()->setMaxAngularVelocity(physx::PxReal(10.0f));
	//Component->GetDynamic()->setAngularDamping(physx::PxReal(0.01f));
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

float Delta = 0.0f;

void TestObject::UserUpdate(float _DeltaTime)
{
	Delta += _DeltaTime;

	float4 ActorPos = GetTransform()->GetWorldPosition();
	float CameraPosyf = ActorPos.y + 40.0f;
	float CameraPoszf = ActorPos.z - 100.0f;

	GetLevel()->GetCamera(0)->GetTransform()->SetWorldPosition(float4{ ActorPos.x, CameraPosyf, CameraPoszf });
	float4 MoveDir = float4::ZERO;
	
	float Set = 100.f;

	if (true == GameEngineInput::IsPress("MoveLeft"))
	{
		MoveDir.x -= Set;
	}
	if (true == GameEngineInput::IsPress("MoveRight"))
	{
		MoveDir.x += Set;
	}
	if (true == GameEngineInput::IsPress("MoveUp"))
	{
		MoveDir.y += Set;
	}
	if (true == GameEngineInput::IsPress("MoveDown"))
	{
		MoveDir.y -= Set;
	}
	if (true == GameEngineInput::IsPress("MoveForward"))
	{
		MoveDir.z += Set;
	}
	if (true == GameEngineInput::IsPress("MoveBack"))
	{
		MoveDir.z -= Set;
		
	}
	if (true == GameEngineInput::IsPress("RotateRight"))
	{
		Rot.y += Set;
	}
	if (true == GameEngineInput::IsPress("RotateLeft"))
	{
		Rot.y -= Set;
	}

	if (true == GameEngineInput::IsDown("Jump"))
	{
		Component->SetclearForce();
		Component->SetJump(3000.f);
	}

	Component->SetMove(MoveDir.NormalizeReturn() * 450.0f);
	
	{
		//float4 playerpos = GetLevel()->GetMainCamera()->GetTransform()->GetWorldPosition();
		//float4 playerposeye = GetLevel()->GetMainCamera()->GetTransform()->GetWorldForwardVector();

		//float4 playerpos = GetTransform()->GetWorldPosition();
		//float4 playerposeye = GetTransform()->GetWorldForwardVector();

		//float4 Colleye = float4::ZERO;

		//playerpos.w = 0.0f;
		//playerposeye.w = 0.0f;
		//Colleye.w = 0.0f;

		//GetLevel()->RayCast(playerpos, playerposeye, Colleye);

		//float4 result = Colleye;
		//Renderer2->GetTransform()->SetWorldPosition(result);
		//int a = 0;
	}
}

void TestObject::ServerUpdate(float _DeltaTime)
{

}
