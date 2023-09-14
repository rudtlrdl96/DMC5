#pragma once
#include "EnemyActor_Normal.h"

enum FSM_State_HellCaina
{
	HellCaina_None,
	HellCaina_Idle,           // em0000_Idle_01, �׳� ������

	HellCaina_Menace_Front,   // em0000_menace_01, �������� ����
	HellCaina_Menace_Up,      // em0000_menace_02, ���� �Լ� ����

	HellCaina_Attack_DownUp,      // �Ʒ����� ���� ����
	HellCaina_Attack_DownUp_step, // �Ʒ����� ���� ���� �� ����
	HellCaina_Attack_UpDown,      // ������ �Ʒ��� ����
	HellCaina_Attack_Dash,        // ��������

	HellCaina_Air_Buster,         // ���߿��� ���Ӱ��� ���� ��
	HellCaina_Buster_Start,       // ������� ��ŸƮ
	HellCaina_Buster_Finish,      // ����� ���� �������� ��
	HellCaina_Gun_Buster,
	HellCaina_Gun_Buster_Start,
	HellCaina_Gun_Buster_Finish,
	HellCaina_M_Buster,
};

// ���� :
class Enemy_HellCaina : public EnemyActor_Normal
{
public:
	// constrcuter destructer
	Enemy_HellCaina();
	~Enemy_HellCaina();

	// delete Function
	Enemy_HellCaina(const Enemy_HellCaina& _Other) = delete;
	Enemy_HellCaina(Enemy_HellCaina&& _Other) noexcept = delete;
	Enemy_HellCaina& operator=(const Enemy_HellCaina& _Other) = delete;
	Enemy_HellCaina& operator=(Enemy_HellCaina&& _Other) noexcept = delete;

	void SetFSMStateValue(int _StateValue) override
	{
		EnemyFSM.ChangeState(_StateValue);
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void EnemyMeshLoad() override;
	void EnemyTypeLoad() override;
	void EnemyAnimationLoad() override;
	void EnemyCreateFSM() override;

private:
	bool CheckBool = false;

	void ChangeState(int _StateValue);	// FSM ���� �Լ�
	bool FloorCheck(float _Distance); // �ٴ�üũ

	float WaitTime = 0.0f;
	float FallCheckDelayTime = 0.0f;
};

