#pragma once

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
	void Update_ProcessPacket() override;
	void Update(float _DeltaTime) override;
	void Update_SendPacket(float _DeltaTime) override;


private:

};

