#pragma once
#include <GameEnginePlatform/GameEngineSound.h>
class BGMPlayer
{
public:
	// constructer destructer
	BGMPlayer();
	~BGMPlayer();

	// delete Function
	BGMPlayer(const BGMPlayer& _Other) = delete;
	BGMPlayer(BGMPlayer&& _Other) = delete;
	BGMPlayer& operator=(const BGMPlayer& _Other) = delete;
	BGMPlayer& operator=(BGMPlayer&& _Other) = delete;

	static GameEngineSoundPlayer SoundPlayer;

	static void SetBattleBGM();

	static void SetBattleEnd();

	static void SetBossBGM();

	static void SetCharater(PlayerType _Type);

	static void BGMLoad();

	static float Volume;

protected:

private:
	static PlayerType Type;
	static int BattleCount;
	static bool BossBGM;
};

