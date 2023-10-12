#pragma once

// Ό³Έν :
class Cavaliere_Electric : public GameEngineActor
{
public:
	// constrcuter destructer
	Cavaliere_Electric();
	~Cavaliere_Electric();

	// delete Function
	Cavaliere_Electric(const Cavaliere_Electric& _Other) = delete;
	Cavaliere_Electric(Cavaliere_Electric&& _Other) noexcept = delete;
	Cavaliere_Electric& operator=(const Cavaliere_Electric& _Other) = delete;
	Cavaliere_Electric& operator=(Cavaliere_Electric&& _Other) noexcept = delete;

	void LookTarget(std::shared_ptr<GameEngineActor> _Target)
	{
		LookTarget(_Target->GetTransform()->GetWorldPosition());
	}
	void LookTarget(const float4& _Target);

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	std::shared_ptr<class FXSystem> Effect = nullptr;
	std::shared_ptr<class AttackCollision> Col = nullptr;
	GameEngineTimeEvent TimeEvent;

	const float Speed = 1000.0f;
};

