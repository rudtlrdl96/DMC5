#include "PrecompileHeader.h"
#include "BGMPlayer.h"

GameEngineSoundPlayer BGMPlayer::SoundPlayer;
PlayerType BGMPlayer::Type = PlayerType::Nero;
float BGMPlayer::Volume = 0.2f;
bool BGMPlayer::BossBGM = false;

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
		SoundPlayer.SetPosition(15.0f);
		SoundPlayer.SoundFadeIn(1.0f);
		SoundPlayer.SetLoop();
		SoundPlayer.SetLoopPoint(198.3f, 360.5f);
		break;
	case PlayerType::Vergil:
		SoundPlayer = GameEngineSound::Play("Bury the Light.ogg");
		SoundPlayer.SetPosition(20.0f);
		SoundPlayer.SoundFadeIn(2.0f);
		SoundPlayer.SetLoop();
		SoundPlayer.SetLoopPoint(51.99f, 359.06f);
		break;
	default:
		break;
	}
	SoundPlayer.SetVolume(Volume);
}

void BGMPlayer::SetBattleEnd()
{
	if (SoundPlayer.IsValid())
	{
		if (true == BossBGM)
		{
			BossBGM = false;
			SoundPlayer.SoundFadeOut(5.0f);
			SoundPlayer = GameEngineSound::Play("Voltaic Black Knight.ogg");
			SoundPlayer.SetVolume(Volume);
			SoundPlayer.SetPosition(207.02f);
			SoundPlayer.SoundFadeIn(0.2f);
			return;
		}

		switch (Type)
		{
		case PlayerType::None:
			break;
		case PlayerType::Nero:
			SoundPlayer.SoundFadeOut(5.0f);
			SoundPlayer = GameEngineSound::Play("Devil Trigger.ogg");
			SoundPlayer.SetVolume(Volume);
			SoundPlayer.SetPosition(361.0f);
			SoundPlayer.SoundFadeIn(0.2f);
			break;
		case PlayerType::Vergil:
			SoundPlayer.SoundFadeOut(5.0f);
			SoundPlayer = GameEngineSound::Play("Bury the Light.ogg");
			SoundPlayer.SetVolume(Volume);
			SoundPlayer.SetPosition(364.0f);
			SoundPlayer.SoundFadeIn(0.2f);
			break;
		default:
			break;
		}
	}
}

void BGMPlayer::SetBossBGM()
{
	BossBGM = true;
	if (SoundPlayer.IsValid())
	{
		SoundPlayer.Stop();
	}

	SoundPlayer = GameEngineSound::Play("Voltaic Black Knight.ogg");
	SoundPlayer.SetVolume(Volume);
	SoundPlayer.SetLoop();
	SoundPlayer.SetLoopPoint(4.17f, 208.7f);
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
