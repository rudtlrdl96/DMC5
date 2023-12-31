#pragma once
#include <GameEngineBase/GameEngineRandom.h>

class SoundController
{
public:
	// constructer destructer
	SoundController();
	~SoundController();

	// delete Function
	SoundController(const SoundController& _Other) = delete;
	SoundController(SoundController&& _Other) = delete;
	SoundController& operator=(const SoundController& _Other) = delete;
	SoundController& operator=(SoundController&& _Other) = delete;

	void Play(const std::string_view& _Name, int _Number = -1)
	{
		if (_Number == -1)
		{
			GameEngineSound::Play(std::string(_Name) + ".wav").SetVolume(SFXVolume);
			return;
		}
		GameEngineSound::Play(std::string(_Name) + std::to_string(_Number) + ".wav").SetVolume(SFXVolume);
	}

	bool IsVoicePlay()
	{
		if (true == VoicePlayer.IsValid())
		{
			bool IsPlaying = false;
			VoicePlayer.isPlaying(&IsPlaying);
			return IsPlaying;
		}
		return false;
	}

	void VoiceStop()
	{
		if (true == IsVoicePlay())
		{
			VoicePlayer.Stop();
		}
	}

	void PlaySetVolume(const std::string_view& _Name, int _Number, float _Volume)
	{
		GameEngineSound::Play(std::string(_Name) + std::to_string(_Number) + ".wav").SetVolume(SFXVolume * _Volume);
	}

	void PlayRandom(const std::string_view& _Name, int _Min, int _Max)
	{
		Play(_Name, GameEngineRandom::MainRandom.RandomInt(_Min, _Max));
	}

	void SetVoiceName(const std::string& _Name)
	{
		VoiceName = _Name;
	}

	void PlayVoice(int _Number, bool _DTValue = false)
	{
		if (true == IsVoicePlay())
		{
			if (true == NoSkip) { return; }
			// 재생 시작된지 0.5초 미만이라면 재생안함
			if (VoicePlayer.getPosition() < 500) { return; }
			VoiceStop();
		}
		NoSkip = false;
		if (true == _DTValue)
		{
			VoicePlayer = GameEngineSound::Play(VoiceName + "DT_" + std::to_string(_Number) + ".wav");
		}
		else
		{
			VoicePlayer = GameEngineSound::Play(VoiceName + std::to_string(_Number) + ".wav");
		}

		VoicePlayer.SetVolume(VoiceVolume);
	}

	void PlayVoiceRandom(int _Min, int _Max, bool _DTValue = false)
	{
		PlayVoice(GameEngineRandom::MainRandom.RandomInt(_Min, _Max), _DTValue);
	}

	void NoSkipOn()
	{
		NoSkip = true;
	}

	float SFXVolume = 1.0f;
	float VoiceVolume = 1.0f;
protected:

private:
	bool NoSkip = false;
	GameEngineSoundPlayer VoicePlayer;
	std::string VoiceName;
};

