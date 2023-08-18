#pragma once
#include "EnemyActor_Boss.h"

// ���� :
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
	void EnemyTextureLoad() override;
	void EnemyAnimationLoad() override;
	void EnemyCreateFSM() override;

	bool IsPowerUpValue = false;

	/*//////////////////////////////////
			�Ϲ� ���� ����
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
				��ȭ ��¡ ����
	//////////////////////////////////*/

	void PowerUP_Enter();
	void PowerUP_Update(float _DeltaTime);
	void PowerUP_Eixt();

	/*//////////////////////////////////
				��ȭ ���� ����
	//////////////////////////////////*/

	// ��ȭ���� �ɾ�� ����ü ������ �ϴ� ����
	void WalkRangeAttack_Enter();
	void WalkRangeAttack_Update(float _DeltaTime);
	void WalkRangeAttack_Exit();

	// ��ȭ���� Į�� �ֵθ��� ����
	void ComboSwordAttack_Enter();
	void ComboSwordAttack_Update(float _DeltaTime);
	void ComboSwordAttack_Exit();
};