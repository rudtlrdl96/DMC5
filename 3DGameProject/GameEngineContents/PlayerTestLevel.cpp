#include "PrecompileHeader.h"
#include "PlayerTestLevel.h"
#include <GameEngineCore/GameEngineFBXMesh.h>
#include "BasePlayerActor.h"
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
	CreateActor<BasePlayerActor>();
}

void PlayerTestLevel::LevelChangeEnd()
{
}
