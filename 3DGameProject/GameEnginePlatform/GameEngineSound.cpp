#include "PrecompileHeader.h"
#include "GameEngineSound.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEngineBase/GameEngineString.h>

// �ٸ� lib�� dll�� ����ϱ� ���� ��ó������ ���� �������Դϴ�.
// #pragma comment(lib, "GameEngineBase.lib");

// lib�� ���ؼ� dll�� ����Ѵٰ� �ϴ°�.
// fmod.dll�� ���ڴ�.
#ifdef _DEBUG
#pragma comment(lib, "..\\GameEnginePlatform\\ThirdParty\\FMOD\\lib\\x64\\fmodL_vc.lib")
#else
#pragma comment(lib, "..\\GameEnginePlatform\\ThirdParty\\FMOD\\lib\\x64\\fmod_vc.lib")
#endif

//////////////////////////// GameEngineSoundPlayer ////////////////////////////

void GameEngineSoundPlayer::SoundFadeIn(double _Time, float _Volume)
{
	if (false == IsValid())
	{
		MsgAssert("��ȿ���� ���� ���带 ���̵�-�� �Ϸ� �߽��ϴ�.");
		return;
	}

	int Rate = 0;
	unsigned long long ParentClock = 0u;

	FMOD::System* SoundSys = nullptr;
	Channel->getSystemObject(&SoundSys);
	SoundSys->getSoftwareFormat(&Rate, 0, 0);
	Channel->getDSPClock(nullptr, &ParentClock);

	unsigned long long EndClock = static_cast<unsigned long long>(ParentClock + (Rate * _Time));

	Channel->removeFadePoints(0, INT64_MAX);
	Channel->addFadePoint(ParentClock, 0.0f);
	Channel->addFadePoint(EndClock, _Volume);
	Channel->setDelay(0, 0, false);
	Channel->setPaused(false);
}

void GameEngineSoundPlayer::SoundFadeOut(double _Time, float _Volume, bool _IsStop /*= false*/)
{
	if (false == IsValid())
	{
		MsgAssert("��ȿ���� ���� ���带 ���̵�-�ƿ� �Ϸ� �߽��ϴ�.");
		return;
	}

	int Rate = 0;
	unsigned long long ParentClock = 0u;
	float CurVolume = 0.0f;

	Channel->setLoopCount(1);
	FMOD::System* SoundSys = nullptr;
	Channel->getSystemObject(&SoundSys);
	SoundSys->getSoftwareFormat(&Rate, 0, 0);
	Channel->getDSPClock(nullptr, &ParentClock);
	Channel->getVolume(&CurVolume);

	unsigned long long EndClock = static_cast<unsigned long long>(ParentClock + (Rate * _Time));

	Channel->removeFadePoints(0, INT64_MAX);
	Channel->addFadePoint(ParentClock, CurVolume);
	Channel->addFadePoint(EndClock, _Volume);
	Channel->setDelay(ParentClock, EndClock, _IsStop);
}

//////////////////////////// GameEngineSound ////////////////////////////

// FMOD�� �ڽŵ��� ����� �̿��Ҽ� �ְ� ���ִ� Ŭ������ �����͸� �ְ�
FMOD::System* SoundSystem = nullptr;

std::unordered_map<std::string, std::shared_ptr<GameEngineSound>> GameEngineSound::AllSound;

class SoundSystemCreator
{
public:
	SoundSystemCreator()
	{
		FMOD::System_Create(&SoundSystem);

		if (nullptr == SoundSystem)
		{
			MsgAssert("���� �ʱ�ȭ�� ����� ���� �ʾҽ��ϴ�.");
		}

		if (FMOD_OK != SoundSystem->init(32, FMOD_DEFAULT, nullptr))
		{
			MsgAssert("���� �ý��� �̴ϼȶ���� �����߽��ϴ�.");
		}

	}

	~SoundSystemCreator()
	{
		SoundSystem->release();
	}
};

SoundSystemCreator SoundInitObject = SoundSystemCreator();

void GameEngineSound::SoundUpdate()
{
	if (nullptr == SoundSystem)
	{
		MsgAssert("���� ������Ʈ�� �Ұ����մϴ�.");
		return;
	}

	SoundSystem->update();
}

GameEngineSound::GameEngineSound()
{
}

GameEngineSound::~GameEngineSound()
{
	if (nullptr == FMODSound)
	{
		FMODSound->release();
	}
}

void GameEngineSound::Load(const std::string_view& _Path)
{
	GameEnginePath NewPath(_Path);
	return Load(NewPath.GetFileName(), _Path);
}

void GameEngineSound::Load(const std::string_view& _Name, const std::string_view& _Path)
{
	std::string UpperName = GameEngineString::ToUpper(_Name);
	std::shared_ptr<GameEngineSound> NewSound = std::make_shared<GameEngineSound>();
	NewSound->SoundLoad(_Path);
	AllSound.insert(std::pair<std::string, std::shared_ptr<GameEngineSound>>(UpperName, NewSound));
}

GameEngineSoundPlayer GameEngineSound::Play(const std::string_view& _Name)
{
	std::string UpperName = GameEngineString::ToUpper(_Name);

	std::unordered_map<std::string, std::shared_ptr<GameEngineSound>>::iterator Finditer = AllSound.find(UpperName);

	if (Finditer == AllSound.end())
	{
		MsgAssert("�������� �ʴ� ���带 �÷����Ϸ��� �߽��ϴ�.");
		return nullptr;
	}
	FMOD::Channel* Channel = Finditer->second->SoundPlay();
	Channel->setLoopCount(0);
	return Channel;
}

void GameEngineSound::SoundLoad(const std::string_view& _Path)
{
	std::string UTF8Path = GameEngineString::AnsiToUTF8(_Path);

	if (FMOD_OK != SoundSystem->createSound(UTF8Path.c_str(), FMOD_LOOP_NORMAL, nullptr, &FMODSound))
	{
		MsgAssert(std::string("���� �ε忡 �����߽��ϴ�.\n ��� : ") + _Path.data());
	}

	return;
}


FMOD::Channel* GameEngineSound::SoundPlay()
{
	if (nullptr == FMODSound)
	{
		MsgAssert(std::string("FMODSound�� nullptr �Դϴ� "));
	}

	FMOD::Channel* Return;

	SoundSystem->playSound(FMODSound, nullptr, false, &Return);

	return Return;
}

void GameEngineSound::ResourcesClear()
{
	AllSound.clear();
}