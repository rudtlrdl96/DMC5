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
			GameEngineSound::Play(std::string(_Name) + ".wav").SetVolume(VFXVolume);
			return;
		}
		GameEngineSound::Play(std::string(_Name) + std::to_string(_Number) + ".wav").SetVolume(VFXVolume);
	}

	void PlaySetVolume(const std::string_view& _Name, int _Number, float _Volume)
	{
		GameEngineSound::Play(std::string(_Name) + std::to_string(_Number) + ".wav").SetVolume(VFXVolume * _Volume);
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
		if (true == VoicePlayer.IsValid())
		{
			bool IsPlaying = false;
			VoicePlayer.isPlaying(&IsPlaying);
			if (true == IsPlaying)
			{
				VoicePlayer.Stop();
			}
		}
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

	float VFXVolume = 1.0f;
	float VoiceVolume = 1.0f;
protected:

private:
	GameEngineSoundPlayer VoicePlayer;
	std::string VoiceName;
};

