#pragma once
#include <GameEnginePlatform/ThirdParty/FMOD/inc/fmod.hpp>
#include <string_view>
#include <unordered_map> 

class GameEngineSoundPlayer
{
private:
	FMOD::Channel* Channel = nullptr;

public:
	GameEngineSoundPlayer()
	{

	}

	GameEngineSoundPlayer(FMOD::Channel* _Channel)
		: Channel(_Channel)
	{

	}

	inline bool IsValid() const
	{
		return (nullptr != Channel);
	}

	inline void isPlaying(bool* _IsPlaying) const
	{
		if (false == IsValid())
		{
			MsgAssert("��ȿ���� ���� ������ ���� ���θ� Ȯ���Ϸ� �߽��ϴ�.");
			return;
		}

		Channel->isPlaying(_IsPlaying);
	}

	void Stop()
	{
		Channel->stop();
	}
	void SetPitch(float _Pitch)
	{
		//����ӵ� ����
		Channel->setPitch(_Pitch);
	}
	void SetVolume(float _Volume)
	{
		Channel->setVolume(_Volume);
	}
	void SetLoop(int _Count = -1)
	{
		// -1 -> ���ѷ��� / 0 -> ����X
		Channel->setLoopCount(_Count);
	}
	void SetMute(bool _Value)
	{
		Channel->setMute(_Value);
	}
	void SetPause(bool _Value)
	{
		Channel->setPaused(_Value);
	}
	void SetPosition(float _StartSecond, FMOD_TIMEUNIT _TimeUnit = FMOD_TIMEUNIT_MS)
	{
		//float���� _StartSecond�� �־��ָ� ms�� �����ؼ� �ش� �������� ����
		Channel->setPosition(static_cast<UINT>(_StartSecond * 1000.f), _TimeUnit);
	}
	void SetLoopPoint(float _StartSecond, float _EndSecond, FMOD_TIMEUNIT _TimeUnit = FMOD_TIMEUNIT_MS)
	{
		//_StartSecond ~_EndSecond ���� �ݺ�
		Channel->setLoopPoints(static_cast<UINT>(_StartSecond * 1000.f), _TimeUnit, static_cast<UINT>(_EndSecond * 1000.f), _TimeUnit);
	}

	/// <param name="_Position">���� ��ġ ��</param>
/// <param name="_TimeType">�Է� ���� Ÿ���� ���մϴ�</param>
	inline void setPosition(UINT _Position, FMOD_TIMEUNIT _TimeType = FMOD_TIMEUNIT_MS)
	{
		if (false == IsValid())
		{
			MsgAssert("��ȿ���� ���� ������ ��ġ�� �����Ϸ� �߽��ϴ�");
			return;
		}

		Channel->setPosition(_Position, _TimeType);
	}

	/// <param name="_TimeType">�Է� ���� Ÿ���� ���մϴ�</param>
	inline UINT getPosition(FMOD_TIMEUNIT _TimeType = FMOD_TIMEUNIT_MS)
	{
		UINT ResultPosition = 0;

		if (false == IsValid())
		{
			MsgAssert("��ȿ���� ���� ������ ��ġ�� �޾ƿ��� �߽��ϴ�");
			return ResultPosition;
		}

		Channel->getPosition(&ResultPosition, _TimeType);
		return ResultPosition;
	}

	/// <summary>
	/// ���带 ������ Ű��ϴ�
	/// </summary>	
	/// <param name="_Volume">����� ���� ������ �����մϴ� �⺻������ 1.0f �Դϴ�</param>
	void SoundFadeIn(double _Time, float _Volume = 1.0f);

	/// <summary>
	/// ���带 ������ ���Դϴ�
	/// </summary>
	/// <param name="_Volume">�������� ���� ������ �����մϴ� �⺻������ 0.0f �Դϴ�</param>
	/// <param name="_IsStop">true�� ��� ���带 Stop �մϴ�, false ��� ���带 �Ͻ����� ��ŵ�ϴ�</param>
	void SoundFadeOut(double _Time, float _Volume = 0.0f, bool _IsStop = false);

};

// ���� :
// �̳༮ ��ü�� ���� ���� ���ҵ� �մϴ�.
class GameEngineSound
{
private: // manager
	static std::unordered_map<std::string, std::shared_ptr<GameEngineSound>> AllSound;


public:
	static void ResourcesClear();

	static void SoundUpdate();

	GameEngineSound();
	~GameEngineSound();

	// delete Function
	GameEngineSound(const GameEngineSound& _Other) = delete;
	GameEngineSound(GameEngineSound&& _Other) noexcept = delete;
	GameEngineSound& operator=(const GameEngineSound& _Other) = delete;
	GameEngineSound& operator=(GameEngineSound&& _Other) noexcept = delete;

	static void Load(const std::string_view& _Path);

	static void Load(const std::string_view& _Name, const std::string_view& _Path);

	static GameEngineSoundPlayer Play(const std::string_view& _Name);

protected:

private:
	void SoundLoad(const std::string_view& _Path);
	FMOD::Channel* SoundPlay();
	// ������
	// ����
	// ���

	// map ����� ���� �����̳�
	// Hash��� ���Դϴ�. 

	FMOD::Sound* FMODSound = nullptr;

};

