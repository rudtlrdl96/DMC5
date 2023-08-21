#include "PrecompileHeader.h"
#include "PlayerTestLevel.h"
#include <GameEngineCore/GameEngineFBXMesh.h>
#include <GameEngineCore/GameEngineSpriteRenderer.h>
#include "BasePlayerActor.h"
#include "AnimationToolWindow.h"
#include "ObjectWindow.h"
#include "FreeCameraActor.h"
#include <GameEngineCore/GameEngineCollision.h>
PlayerTestLevel::PlayerTestLevel() 
{
}

PlayerTestLevel::~PlayerTestLevel() 
{
}

void PlayerTestLevel::Start()
{
	GetMainCamera()->SetProjectionType(CameraType::Perspective);
	GetMainCamera()->GetTransform()->SetLocalPosition({ 0, 50, -500 });

	
}

void PlayerTestLevel::Update(float _DeltaTime)
{
	if (false == IsMessage)
	{
		IsMessage = true;
		MsgTextBox("CurrentLevel is PlayerTestLevel");
	}
	if (true == GameEngineInput::IsDown("ReturnToMainLevel"))
	{
		GameEngineCore::ChangeLevel("MainLevel");
	}
}

void PlayerTestLevel::LevelChangeStart()
{
	if (nullptr != BasePlayerActor::MainPlayer)
	{
		return;
	}
	if (nullptr == GameEngineFBXMesh::Find("Nero.fbx")) {
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Mesh");
		NewDir.Move("Characters");
		// �׽�Ʈ �޽� ������ �ڵ����� �ε��մϴ�
		std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".FBX" });

		for (size_t i = 0; i < Files.size(); i++)
		{
			//GameEngineFBXMesh::Load(Files[i].GetFullPath());
		}
	}
	std::shared_ptr<BasePlayerActor> NewPlayer = CreateActor<BasePlayerActor>();
	NewPlayer->SetName("Nero");
	//CreateActor<BasePlayerActor>();
	std::shared_ptr<GameEngineActor> Actor = CreateActor<GameEngineActor>();
	Actor->GetTransform()->SetWorldScale({ 1000, 1000, 1 });
	Actor->GetTransform()->SetWorldPosition({ 0, 0, 500 });
	Actor->CreateComponent<GameEngineSpriteRenderer>();

	std::shared_ptr<GameEngineActor> CollisionActor = CreateActor<GameEngineActor>();
	std::shared_ptr<GameEngineCollision> Collision = CollisionActor->CreateComponent<GameEngineCollision>();
	CollisionActor->GetTransform()->SetLocalScale({ 100, 100, 100 });
	Collision->SetColType(ColType::AABBBOX3D);

	IsDebugSwitch();

	AnimWindow = std::dynamic_pointer_cast<AnimationToolWindow>(GameEngineGUI::GUIWindowCreate<AnimationToolWindow>("AnimationToolWindow"));
	ObjWindow =  std::dynamic_pointer_cast<ObjectWindow>(GameEngineGUI::GUIWindowCreate<ObjectWindow>("ObjectWindow"));
	ObjWindow->AddObject(NewPlayer);
	ObjWindow->AddObject(CollisionActor);
	ObjWindow->AddObject(Collision);
	std::shared_ptr<FreeCameraActor> Free = CreateActor<FreeCameraActor>();
}

void PlayerTestLevel::LevelChangeEnd()
{
}