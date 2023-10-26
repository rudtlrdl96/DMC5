#include "PrecompileHeader.h"
#include "BGMPlayer.h"

GameEngineSoundPlayer BGMPlayer::SoundPlayer;
PlayerType BGMPlayer::Type = PlayerType::Nero;
int BGMPlayer::BattleCount = 0;
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
	if (nullptr == GameEngineSound::Find("Devil Trigger.ogg")) { BGMLoad(); }
	if (SoundPlayer.IsValid())
	{
		SoundPlayer.Stop();
	}

	switch (Type)
	{
	case PlayerType::None:
		break;
	case PlayerType::Nero:
	{
		SoundPlayer = GameEngineSound::Play("Devil Trigger.ogg");
		SoundPlayer.SoundFadeIn(1.0f);
		SoundPlayer.SetLoop();
		SoundPlayer.SetLoopPoint(198.3f, 360.5f);
		if (BattleCount == 0)
		{
			SoundPlayer.SetPosition(13.0f);
		}
		else if (BattleCount == 1)
		{
			SoundPlayer.SetPosition(33.0f);
		}
		else
		{
			SoundPlayer.SetPosition(62.5f);
		}
		break;
	}
	case PlayerType::Vergil:
	{
		SoundPlayer = GameEngineSound::Play("Bury the Light.ogg");
		SoundPlayer.SoundFadeIn(2.0f);
		SoundPlayer.SetLoop();
		SoundPlayer.SetLoopPoint(51.99f, 359.06f);
		if (BattleCount == 0)
		{
			SoundPlayer.SetPosition(20.0f);
		}
		else if (BattleCount == 1)
		{
			SoundPlayer.SetPosition(67.8f);
		}
		else
		{
			SoundPlayer.SetPosition(93.0f);
		}
		break;
	}
	default:
		break;
	}
	SoundPlayer.SetVolume(Volume);
	BattleCount++;
}

void BGMPlayer::SetBattleEnd()
{
	if (nullptr == GameEngineSound::Find("Devil Trigger.ogg")) { BGMLoad(); }
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
	if (nullptr == GameEngineSound::Find("Devil Trigger.ogg")) { BGMLoad(); }

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

void BGMPlayer::SetTitleBGM()
{
	if (nullptr == GameEngineSound::Find("Devil Trigger.ogg")) { BGMLoad(); }

	SoundPlayer = GameEngineSound::Play("Titlescreen.ogg");
	SoundPlayer.SetVolume(Volume);
	SoundPlayer.SetPosition(15.0f);
	SoundPlayer.SoundFadeIn(0.5f);
	SoundPlayer.SetLoop();
	SoundPlayer.SetLoopPoint(20.0f, 100.f);
}

void BGMPlayer::SetBGMFadeOut()
{
	if (SoundPlayer.IsValid())
	{
		SoundPlayer.SoundFadeOut(1.0f);
	}
}

void BGMPlayer::SetCharater(PlayerType _Type)
{
	if (Type == _Type) { return; }
	Type = _Type;
	BattleCount = 0;
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
