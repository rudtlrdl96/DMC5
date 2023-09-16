#pragma once
#include "EnemyActor_Normal.h"

enum FSM_State_HellCaina
{
	HellCaina_None,
	HellCaina_Idle,           // em0000_Idle_01, �׳� ������

	HellCaina_Walk_Start,
	HellCaina_Walk_Loop,
	HellCaina_Walk_Stop,

	HellCaina_Turn_Left_90,   // ���� ȸ��
	HellCaina_Turn_Left_180,
	HellCaina_Turn_Right_90,  // ������ ȸ��
	HellCaina_Turn_Right_180,

	HellCaina_Standing_Damage_Weak_Front, // = ���� ����� ��Ʈ
	HellCaina_Standing_Damage_Weak_Back, // = �ĸ� ����� ��Ʈ
	HellCaina_Standing_Damage_Weak_Left, // = ���� ����� ��Ʈ
	HellCaina_Standing_Damage_Weak_Right, // = ���� ����� ��Ʈ

	//HellCaina_Blown_Front = ���ư��鼭 ������ ��ٶ���
	//HellCaina_Blown_Front_loop = ���ư��鼭 ������ ��ٶ��� ����
	HellCaina_Blown_Back, // = ���ư��鼭 �ڷ� ��ٶ���
	HellCaina_Blown_Back_Under, // = ���ư��鼭 �ڷ� ��ٶ��� �ְ��� ������ �ִϸ��̼�
	HellCaina_Blown_Back_Loop, //  = ���ư��鼭 �ڷ� ��ٶ��� ����
	HellCaina_Blown_Back_Landing, // = ���ư��鼭 �ڷ� ��ٶ��� ����

	HellCaina_Lie_Getup,  // �ڷ� �������� �� �Ͼ�� ���
	HellCaina_Prone_Getup, // ������ �������� �� �Ͼ�� ���

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

	void DamageCollisionCheck() override;

private:
	bool CheckBool = false;
	bool AttackCheck = false;

	void ChangeState(int _StateValue);	// FSM ���� �Լ�
	void TurnToPlayer(float _DeltaTime);
	void RotationToPlayer(float _DeltaTime);

	float WaitTime = 0.0f;
	float WalkTime = 0.0f;
	float RotationDelayTime = 0.0f;
	float FallCheckDelayTime = 0.0f;

	float4 MoveDir = float4::ZERO;
};

