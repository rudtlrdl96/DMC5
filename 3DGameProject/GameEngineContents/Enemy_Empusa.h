#pragma once
enum FSM_State_Empusa
{
	//Idle
	Empusa_Idle,

	// 움직임
	Empusa_Walk_Start,       // em0100_walk_start
	Empusa_Walk_Loop,        // em0100_walk_loop
	Empusa_Walk_Stop,        // em0100_walk_stop
	Empusa_Walk_Right_Start, // em0100_walk_right_start
	Empusa_Walk_Right_Loop,  // em0100_walk_right_loop
	Empusa_Walk_Right_Stop,  // em0100_walk_right_stop
	Empusa_Walk_Left_Start,  // em0100_walk_left_start
	Empusa_Walk_Left_Loop,   // em0100_walk_left_loop
	Empusa_Walk_Left_Stop,   // em0100_walk_left_stop

	// 회전
	Empusa_Turn_Left,  // em0100
	Empusa_Turn_Right, // em0100
	Empusa_Turn_Left_90,    // em0100_turn_left_90, 왼쪽 회전
	Empusa_Turn_Right_90,   // em0100_turn_right_90, 오른쪽 회전
	Empusa_Turn_Left_180,   // em0100_turn_left_180, 왼쪽 회전
	Empusa_Turn_Right_180,  // em0100_turn_right_180, 오른쪽 회전
};
class Enemy_Empusa : public BaseEnemyActor
{
public:
	// constrcuter destructer
	Enemy_Empusa();
	~Enemy_Empusa();

	// delete Function
	Enemy_Empusa(const Enemy_Empusa& _Other) = delete;
	Enemy_Empusa(Enemy_Empusa&& _Other) noexcept = delete;
	Enemy_Empusa& operator=(const Enemy_Empusa& _Other) = delete;
	Enemy_Empusa& operator=(Enemy_Empusa&& _Other) noexcept = delete;
	
	void PlayerChase(float _DeltaTime);
	void ChangeState(int _StateValue);
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

	float4 MoveDir = float4::ZERO;
	float4 PushDir = float4::ZERO;
};

