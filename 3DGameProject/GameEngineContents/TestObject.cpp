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
	}
	if (false == GameEngineInput::IsKey("Jump"))
	{
		GameEngineInput::CreateKey("Jump", VK_SPACE);
	}

	//GetTransform()->AddWorldPosition(float4{ 0, 100.0f, 0 });

	Renderer = CreateComponent<GameEngineFBXRenderer>();
	Renderer->SetFBXMesh("Knight.fbx", "FBX");
	Renderer->GetTransform()->SetLocalScale({ 0.1f , 0.1f , 0.1f });
	//if (nullptr == GameEngineFBXMesh::Find("em0100.FBX"))
	//{
	//	std::string Path = GameEnginePath::GetFileFullPath
	//	(
	//		"ContentResources",
	//		{
	//			"Character", "Enemy", "em0100", "mesh"
	//		},
	//		"em0100.FBX"
	//	);
	//	GameEngineFBXMesh::Load(Path);
	//}

	//Renderer->SetFBXMesh("em0100.fbx", "FBX");
	//Renderer->GetTransform()->SetLocalScale({ 0.1f , 0.1f , 0.1f });

	float4 RenderScale = Renderer->GetFBXMesh()->GetRenderUnit(0)->BoundScaleBox;
	float4 MeshScale = Renderer->GetMeshScale();

	physx::PxVec3 VecSclae = { RenderScale.x, RenderScale.y, RenderScale.z };


	//Component = CreateComponent<PhysXCapsuleComponent>();
	//Component->SetPhysxMaterial(0.0f, 0.0f, 0.0f);
	//Component->CreatePhysXActors(GetLevel()->GetLevelScene(), GetLevel()->GetLevelPhysics(), VecSclae * 0.1f);
	
	TriCom = CreateComponent<PhysXTriangleComponent>();
	TriCom->SetPhysxMaterial(0.0f, 0.0f, 0.0f);
	TriCom->CreatePhysXActors("Knight.fbx", GetLevel()->GetLevelScene(), GetLevel()->GetLevelPhysics(), GetLevel()->GetCooking(), false, VecSclae * 0.1f);

	std::shared_ptr<GameEngineFBXRenderer> Renderer = CreateComponent<GameEngineFBXRenderer>();
	//Renderer->SetFBXMesh("Nero.FBX", "MeshTexture");
	//Renderer->CreateFBXAnimation("Dash", "pl0000_Dash_Loop.FBX");
	//Renderer->ChangeAnimation("Dash");

	//Renderer->SetFBXMesh("Nero.FBX", "MeshTexture");
	//Renderer->CreateFBXAnimation("Dash", "pl0000_Dash_Loop.FBX");
	//Renderer->ChangeAnimation("Dash");

	//Renderer->SetFBXMesh("AnimMan.FBX", "MeshAniTexture");
	//Renderer->CreateFBXAnimation("Run", "ALS_N_Run_F.FBX");
	//Renderer->ChangeAnimation("Run");

	//{
	//	std::shared_ptr<GameEngineFontRenderer> Renderer = CreateComponent<GameEngineFontRenderer>();
	//	Renderer->SetScale(100.0f);
	//	Renderer->SetFont("�ü�");
	//	Renderer->SetText("aaaaaaaaaa");
	//}

	//Component->GetDynamic()->setMass(10.f);
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

	//if (2.0f <= Delta)
	//{
	//	Component->TurnOffGravity();
	//}

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
		//Component->SetclearForce();
		//Component->SetJump(3000.f);
	}

	//Component->SetMove(MoveDir.NormalizeReturn() * 500.0f);
	
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
