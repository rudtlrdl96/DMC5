#pragma once

// 설명 :
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

	// 이 엑터를 _Target을 향하게 방향을 전환합니다
	void LookTarget(GameEngineActor* _Target)
	{
		LookTarget(_Target->GetTransform()->GetWorldPosition());
	}
	// 이 엑터를 _Target을 향하게 방향을 전환합니다
	void LookTarget(const float4& _Target);

	// 번개 발사하는 함수. _Target 방향으로 날라갑니다
	void Shoot(const float4& _Target)
	{
		Shoot();
		LookTarget(_Target);
	}
	// 번개 발사하는 함수. _Target 방향으로 날라갑니다
	void Shoot(GameEngineActor* _Target)
	{
		Shoot();
		LookTarget(_Target);
	}
	// 번개 발사하는 함수. 이 엑터의 Forward 방향으로 날라갑니다
	void Shoot();

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	std::shared_ptr<class FXSystem> Effect = nullptr;
	std::shared_ptr<class AttackCollision> Col = nullptr;
	GameEngineTimeEvent TimeEvent;

	const float Speed = 7000.0f;
	bool IsShoot = false;
};

