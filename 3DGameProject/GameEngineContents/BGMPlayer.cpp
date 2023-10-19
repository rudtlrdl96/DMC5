#include "PrecompileHeader.h"
#include "BGMPlayer.h"

GameEngineSoundPlayer BGMPlayer::SoundPlayer;
PlayerType BGMPlayer::Type = PlayerType::None;
float BGMPlayer::Volume = 0.3f;

BGMPlayer::BGMPlayer()
{

}

BGMPlayer::~BGMPlayer()
{
}

void BGMPlayer::SetBattleBGM()
{
	if (SoundPlayer.IsValid())
	{
		SoundPlayer.Stop();
	}

	switch (Type)
	{
	case PlayerType::None:
		break;
	case PlayerType::Nero:
		SoundPlayer = GameEngineSound::Play("Devil Trigger.ogg");
		break;
	case PlayerType::Vergil:
		SoundPlayer = GameEngineSound::Play("Bury the Light.ogg");
		break;
	default:
		break;
	}
	SoundPlayer.SetVolume(Volume);
}

void BGMPlayer::SetBossBGM()
{
	if (SoundPlayer.IsValid())
	{
		SoundPlayer.Stop();
	}

	SoundPlayer = GameEngineSound::Play("Voltaic Black Knight.ogg");
	SoundPlayer.SetVolume(Volume);
}

void BGMPlayer::SetCharater(PlayerType _Type)
{
	Type = _Type;
}

void BGMPlayer::BGMLoad()
{
	if (nullptr != GameEngineSound::Find("Devil Trigger.ogg")) { return; }

	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Sound");
	NewDir.Move("BGM");
	std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".ogg" });
	for (GameEngineFile File : Files)
	{
		GameEngineSound::Load(File.GetFullPath());
	}

}
