#include "PrecompileHeader.h"
#include "PlayUITestLevel.h"
#include "NeroHPUI.h"

#include <GameEngineCore/GameEngineFBXMesh.h>

PlayUITestLevel::PlayUITestLevel()
{
}

PlayUITestLevel::~PlayUITestLevel()
{
}

void PlayUITestLevel::Start()
{
	{
		std::string Path = GameEnginePath::GetFileFullPath
		(
			"ContentResources",
			{
				"Mesh", "UIMesh"
			},
			"NeroHPUI.fbx"
		);

		GameEngineFBXMesh::Load(Path);
	}

	GetMainCamera()->SetProjectionType(CameraType::Perspective);
	GetMainCamera()->GetTransform()->SetLocalPosition({ 0, 0, -500.0f });
	std::shared_ptr<NeroHPUI> NeroHPPtr = CreateActor<NeroHPUI>();
	NeroHPPtr->GetTransform()->SetLocalPosition({ 0.0f,0.0f,-400.0f });
}

void PlayUITestLevel::Update(float _DeltaTime)
{
	BaseLevel::Update(_DeltaTime);

}

void PlayUITestLevel::LevelChangeStart()
{

	GameEngineLevel::LevelChangeStart();
}

void PlayUITestLevel::LevelChangeEnd()
{
	GameEngineLevel::LevelChangeEnd();
}