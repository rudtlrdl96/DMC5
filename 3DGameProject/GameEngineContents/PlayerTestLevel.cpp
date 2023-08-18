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

	if (false == GameEngineInput::IsKey("Up"))
	{
		GameEngineInput::CreateKey("Up", 'W');
		GameEngineInput::CreateKey("Left", 'A');
		GameEngineInput::CreateKey("Down", 'S');
		GameEngineInput::CreateKey("Right", 'D');
		GameEngineInput::CreateKey("Sword", 'I');
		GameEngineInput::CreateKey("Gun", 'J');
		GameEngineInput::CreateKey("Jump", 'K');
		GameEngineInput::CreateKey("Skill", 'L');
		GameEngineInput::CreateKey("Exceed", 'Q');
		GameEngineInput::CreateKey("LockOn", VK_SPACE);
		GameEngineInput::CreateKey("DT", VK_LSHIFT);
		GameEngineInput::CreateKey("CameraLeft", 'U');
		GameEngineInput::CreateKey("CameraRight", 'O');
		GameEngineInput::CreateKey("CameraUp", 'Y');
		GameEngineInput::CreateKey("CameraDown", 'P');
	}

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
	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Mesh");
		NewDir.Move("Characters");
		// 테스트 메쉬 폴더는 자동으로 로드합니다
		std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".FBX" });

		for (size_t i = 0; i < Files.size(); i++)
		{
			GameEngineFBXMesh::Load(Files[i].GetFullPath());
		}
	}

	std::shared_ptr<BasePlayerActor> NewPlayer = CreateActor<BasePlayerActor>();

}

void PlayerTestLevel::LevelChangeEnd()
{
}
