#pragma once

enum FSM_State_CavaliereAngelo
{
	// ����
	CavaliereAngelo_Idle,
};

// ���� :
class CavaliereAngelo : public BaseEnemyActor
{
public:
	// constrcuter destructer
	CavaliereAngelo();
	~CavaliereAngelo();

	// delete Function
	CavaliereAngelo(const CavaliereAngelo& _Other) = delete;
	CavaliereAngelo(CavaliereAngelo&& _Other) noexcept = delete;
	CavaliereAngelo& operator=(const CavaliereAngelo& _Other) = delete;
	CavaliereAngelo& operator=(CavaliereAngelo&& _Other) noexcept = delete;

protected:
	void Start() override;
	void EnemyMeshLoad() override;
	void EnemyTypeLoad() override;
	void EnemyAnimationLoad() override;
	void EnemyCreateFSM() override;
	void EnemyCreateFSM_Client() override;
	void DamageCollisionCheck(float _DeltaTime) override;
	void RecognizeCollisionCheck(float _DeltaTime) override;

private:
	void PlayerChase(float _DeltaTime);
	void PlayerAttack(float _DeltaTime);
	void ChangeState(int _StateValue);

	float WaitTime = 0.0f;
	float RotationDelayTime = 0.0f;
	float FallCheckDelayTime = 0.0f;
	float AttackDelayCheck = 0.0f;
	float AttackDelayTime = 0.0f;

	short WalkCount = 0;

	bool IsHeavyAttack = false;   // ������ ��Ʈ
	bool IsAirAttack = false;     // ������� ��Ʈ
	bool IsSlamAttack = false;    // �������� ��Ʈ
	bool IsBusterAttack = false;  // ������ ��Ʈ
	bool IsVergilLight = false;
	bool IsCollapse = false;      // �������ִ� ����
	bool IsRecognize = false;

	bool IsPowerUpValue = false;

};