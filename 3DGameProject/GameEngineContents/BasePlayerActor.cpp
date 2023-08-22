#include "PrecompileHeader.h"
#include "BasePlayerActor.h"
#include <GameEngineBase/GameEngineNet.h>
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>
#include "PlayerController.h"
#include "PlayerCamera.h"
#include "ServerWindow.h"
#include "ObjectUpdatePacket.h"
#include "ContentsEnum.h"
#include "BaseLog.h"
BasePlayerActor* BasePlayerActor::Instance = nullptr;

BasePlayerActor::BasePlayerActor()
{
	if (GameEngineNetObject::GetControllType() == NetControllType::UserControll)
	{
		Instance = this;
	}
}

BasePlayerActor::~BasePlayerActor()
{
}

void BasePlayerActor::LookDir(const float4& _LookDir)
{
	float4 LocalForward = GetTransform()->GetWorldForwardVector();
	//float4 LocalForward = Controller->GetMoveVector();

	float4 Cross = float4::Cross3DReturnNormal(LocalForward,  _LookDir);
	//float4 Cross = float4::Cross3DReturnNormal(_LookDir, LocalForward);
	float Dot = float4::DotProduct3D(LocalForward, _LookDir);
	//float Dot = float4::DotProduct3D(_LookDir, LocalForward);
	if (Cross.y == -1.0f)
	{
		GetTransform()->AddLocalRotation({ 0, 180, 0 });

	}
	else
	{
		GetTransform()->AddLocalRotation({ 0, -GameEngineMath::RadToDeg * Dot, 0 });
	}
	return;

}

void BasePlayerActor::Start()
{

	//Renderer = CreateComponent<GameEngineFBXRenderer>();
	//Renderer->GetTransform()->SetLocalRotation({ 0, 90, 0 });
	//Renderer->SetFBXMesh("Nero.FBX", "MeshTexture");
	//Renderer->SetFBXMesh("Nero.FBX", "MeshTexture");
	//Renderer->CreateFBXAnimation("Dash", "pl0000_Dash_Loop.FBX");
	//Renderer->ChangeAnimation("Dash");

	Camera = GetLevel()->CreateActor<PlayerCamera>();
	Camera->SetPlayerTranform(GetTransform());
	Controller = CreateComponent<PlayerController>();
	Controller->SetCameraTransform(Camera->GetTransform());

	Controller->CallBack_LockOnDown = std::bind(&BasePlayerActor::LockOn, this);
	Controller->CallBack_LockOnUp = std::bind(&BasePlayerActor::LockOff, this);

	PlayerCollision = CreateComponent<GameEngineCollision>(CollisionOrder::Player);
	PlayerCollision->GetTransform()->SetLocalScale({ 100, 100, 100 });
	PlayerCollision->SetColType(ColType::OBBBOX3D);

	LockOnCollision = CreateComponent<GameEngineCollision>(CollisionOrder::Player);
	LockOnCollision->GetTransform()->SetLocalScale({ 3000, 500, 1000 });
	LockOnCollision->GetTransform()->SetLocalPosition({ 1500, 0, 0 });
	LockOnCollision->SetColType(ColType::OBBBOX3D);
}

void BasePlayerActor::Update(float _DeltaTime)
{
	NetControllType Type = GetControllType();
	GetTransform()->AddLocalPosition(Controller->GetMoveVector() * _DeltaTime * 100);
	if (Controller->GetMoveVector() != float4::ZERO && LockOnEnemyTransform == nullptr)
	{
		LookDir(Controller->GetMoveVector());
	}
	else if (LockOnEnemyTransform != nullptr)
	{
		LookDir((LockOnEnemyTransform->GetWorldPosition() - GetTransform()->GetWorldPosition()).NormalizeReturn());
	}

	static float Delta = 0.0f;
	Delta += _DeltaTime;
	if (Delta <= 1.0f / 60.0f)
	{
		return;
	}

	Delta -= 1.0f / 60.0f;
	if (true == IsNet())
	{
		std::shared_ptr<ObjectUpdatePacket> NewPacket = std::make_shared<ObjectUpdatePacket>();
		NewPacket->SetObjectID(GetNetObjectID());
		NewPacket->Position = GetTransform()->GetWorldPosition();
		NewPacket->Rotation = GetTransform()->GetWorldPosition();
		GetNet()->SendPacket(NewPacket);
	}
}

void BasePlayerActor::LockOn()
{
	std::vector<std::shared_ptr<GameEngineCollision>> Cols;
	std::shared_ptr<GameEngineCollision> MinCol = nullptr;
	if (true == LockOnCollision->CollisionAll(CollisionOrder::Enemy, Cols))
	{
		float Min = 9999;
		for (std::shared_ptr<GameEngineCollision> Col : Cols)
		{
			float Length = (GetTransform()->GetWorldPosition() - Col->GetTransform()->GetWorldPosition()).Size();
			if (Length < Min)
			{
				Min = Length;
				MinCol = Col;
			}
		}
		if (MinCol == nullptr)
		{
			int a = 0;
			return;
		}
		LockOnEnemyTransform = MinCol->GetActor()->GetTransform();
	}
}

void BasePlayerActor::LockOff()
{
	LockOnEnemyTransform = nullptr;
}

