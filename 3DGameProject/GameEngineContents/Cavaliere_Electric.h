#pragma once

// ���� :
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

	// �� ���͸� _Target�� ���ϰ� ������ ��ȯ�մϴ�
	void LookTarget(GameEngineActor* _Target)
	{
		LookTarget(_Target->GetTransform()->GetWorldPosition());
	}
	// �� ���͸� _Target�� ���ϰ� ������ ��ȯ�մϴ�
	void LookTarget(const float4& _Target);

	// ���� �߻��ϴ� �Լ�. _Target �������� ���󰩴ϴ�
	void Shoot(const float4& _Target)
	{
		Shoot();
		LookTarget(_Target);
	}
	// ���� �߻��ϴ� �Լ�. _Target �������� ���󰩴ϴ�
	void Shoot(GameEngineActor* _Target)
	{
		Shoot();
		LookTarget(_Target);
	}
	// ���� �߻��ϴ� �Լ�. �� ������ Forward �������� ���󰩴ϴ�
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

