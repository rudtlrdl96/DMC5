#pragma once
#include "EnemyActor_Boss.h"

// 설명 :
class CavaliereAngelo : public EnemyActor_Boss
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

private:
	void EnemyMeshLoad() override;
	void EnemyTypeLoad() override;
	void EnemyAnimationLoad() override;
	void EnemyCreateFSM() override;

	bool IsPowerUpValue = false;

	/*//////////////////////////////////
			일반 상태 패턴
	//////////////////////////////////*/

	void SwordAttack_Enter();
	void SwordAttack_Update(float _DeltaTime);
	void SwordAttack_Exit();

	void Staggering_Enter();
	void Staggering_Update(float _DeltaTime);
	void Staggering_Exit();

	void Death_Enter();
	void Death_Update(float _DeltaTime);
	void Death_Exit();

	/*//////////////////////////////////
				강화 차징 패턴
	//////////////////////////////////*/

	void PowerUP_Enter();
	void PowerUP_Update(float _DeltaTime);
	void PowerUP_Eixt();

	/*//////////////////////////////////
				강화 상태 패턴
	//////////////////////////////////*/

	// 강화상태 걸어가며 투사체 공격을 하는 패턴
	void WalkRangeAttack_Enter();
	void WalkRangeAttack_Update(float _DeltaTime);
	void WalkRangeAttack_Exit();

	// 강화상태 칼을 휘두르는 패턴
	void ComboSwordAttack_Enter();
	void ComboSwordAttack_Update(float _DeltaTime);
	void ComboSwordAttack_Exit();
};