#include "PrecompileHeader.h"
#include "TestObject.h"

//#include <GameEngineCore/PhysXBoxComponent.h>
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/PhysXCapsuleComponent.h>
#include <GameEngineCore/GameEngineFontRenderer.h>

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

	Renderer->SetFBXMesh("em0100.fbx", "NoneAlphaMesh");
	Renderer->GetTransform()->SetLocalScale({ 0.1f , 0.1f , 0.1f });

	//Renderer->GetAllRenderUnit()[0][0];

	float4 RenderScale = Renderer->GetFBXMesh()->GetRenderUnit(0)->BoundScaleBox;
	float4 MeshScale = Renderer->GetMeshScale();

	physx::PxVec3 VecSclae = { RenderScale.x, RenderScale.y, RenderScale.z };

	float4 world = GetTransform()->GetWorldScale();
	float4 local = GetTransform()->GetLocalScale();

	Component = CreateComponent<PhysXCapsuleComponent>();

	//                          정지  운동  반발
	Component->SetPhysxMaterial(0.0f, 0.0f, 0.0f);
	Component->CreatePhysXActors(GetLevel()->GetLevelScene(), GetLevel()->GetLevelPhysics(), VecSclae * 0.1f);

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
	//	Renderer->SetFont("궁서");
	//	Renderer->SetText("aaaaaaaaaa");
	//}

	Component->GetDynamic()->setMass(5.0f);
	Component->SetSpeedLimitValue(180.0f);
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

void TestObject::UserUpdate(float _DeltaTime)
{
	float4 ActorPos = GetTransform()->GetWorldPosition();
	float CameraPosyf = ActorPos.y + 40.0f;
	float CameraPoszf = ActorPos.z - 100.0f;

	GetLevel()->GetCamera(0)->GetTransform()->SetWorldPosition(float4{ ActorPos.x, CameraPosyf, CameraPoszf });

	physx::PxTransform PhyTF = Component->GetDynamic()->getGlobalPose();
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
		Component->SetJump(100.0f);
	}

	float Speed = 200.0f;
	float Dir = Speed * _DeltaTime;

	Component->SetMove(MoveDir.NormalizeReturn() * 50000.0f * _DeltaTime);
	//Component->GetDynamic()->setLinearVelocity({ MoveDir.x, 0.0f, MoveDir.z });
	//Component->GetDynamic()->setAngularVelocity({0.0f, Rot.y, 0.0f });
	Component->SetWorldRotation(Rot);

	float4 asef = Component->GetDynamicVelocity();
	int a = 0;
}

void TestObject::ServerUpdate(float _DeltaTime)
{

}