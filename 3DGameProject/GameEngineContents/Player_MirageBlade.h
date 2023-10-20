#pragma once
#include "NetworkObjectBase.h"
#include "Poolable.h"

// Ό³Έν :
class Player_MirageBlade : public GameEngineActor, public NetworkObjectBase, public Poolable<Player_MirageBlade>
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

	void On();

	void Reset();
	void SetTarget(GameEngineTransform* _Transform);
	void LookTarget();
	void Shoot();
	void SetSpiral();
	void SpiralStop();
	std::shared_ptr<class AttackCollision> Collision = nullptr;
protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	std::shared_ptr<class EffectRenderer> Renderer = nullptr;
	std::shared_ptr<class FXSystem> Effect = nullptr;
	GameEngineTransform* TargetTransform = nullptr;
	float ShootSpeed = 5000.0f;
	float RenderAlpha = 0.0f;
	int EffectValue = -1;
	bool IsShoot = false;
};

