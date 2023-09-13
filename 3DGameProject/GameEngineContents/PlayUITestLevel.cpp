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

	}

	GetMainCamera()->SetProjectionType(CameraType::Perspective);
	GetMainCamera()->GetTransform()->SetLocalPosition({ 0, 0, -1000.0f });
	std::shared_ptr<NeroHPUI> NeroHPPtr = CreateActor<NeroHPUI>();

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