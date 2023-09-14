#pragma once

#include <GameEngineCore/PhysicsLevel.h>

class MonsterTestLevel : public GameEngineLevel
{
public:
	// construtor destructor
	MonsterTestLevel();
	~MonsterTestLevel();

	// delete Function
	MonsterTestLevel(const MonsterTestLevel& _Other) = delete;
	MonsterTestLevel(MonsterTestLevel&& _Other) noexcept = delete;
	MonsterTestLevel& operator=(const MonsterTestLevel& _Other) = delete;
	MonsterTestLevel& operator=(MonsterTestLevel&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render() {};

	void LevelChangeStart() override;
	void LevelChangeEnd() override;

private:
	bool IsMessage = false;
	std::shared_ptr<class Enemy_Empusa> TestMonster = nullptr;
	std::shared_ptr<class PlayerActor_Nero> TestPlayer = nullptr;
};

