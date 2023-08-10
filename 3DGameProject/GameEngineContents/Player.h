#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineBase/GameEngineNetObject.h>

// Ό³Έν :
class Player : public GameEngineActor, public GameEngineNetObject
{
public:
	static Player* MainPlayer;

public:
	// constrcuter destructer
	Player();
	~Player();

	// delete Function
	Player(const Player& _Other) = delete;
	Player(Player&& _Other) noexcept = delete;
	Player& operator=(const Player& _Other) = delete;
	Player& operator=(Player&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void UserUpdate(float _DeltaTime);
	void ServerUpdate(float _DeltaTime);

private:

};

