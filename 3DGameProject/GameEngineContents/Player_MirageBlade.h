#pragma once

// Ό³Έν :
class Player_MirageBlade : public GameEngineActor, public GameEngineNetObject
{
public:
	// constrcuter destructer
	Player_MirageBlade();
	~Player_MirageBlade();

	// delete Function
	Player_MirageBlade(const Player_MirageBlade& _Other) = delete;
	Player_MirageBlade(Player_MirageBlade&& _Other) noexcept = delete;
	Player_MirageBlade& operator=(const Player_MirageBlade& _Other) = delete;
	Player_MirageBlade& operator=(Player_MirageBlade&& _Other) noexcept = delete;

	void Reset();
	void SetTarget(GameEngineTransform* _Transform);
	void LookTarget();
	void Shoot();

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	std::shared_ptr<class GameEngineFBXRenderer> Renderer = nullptr;
	std::shared_ptr<class AttackCollision> Collision = nullptr;
	GameEngineTransform* TargetTransform = nullptr;
	float ShootSpeed = 5000.0f;
	bool IsShoot = false;

};

