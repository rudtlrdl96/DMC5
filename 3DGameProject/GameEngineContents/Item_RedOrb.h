#pragma once
#include "ContentsEnum.h"
#include "FieldMapObject.h"
// ���� :
class Item_RedOrb : public FieldMapObject
{
public:
	// constrcuter destructer
	Item_RedOrb();
	~Item_RedOrb();

	// delete Function
	Item_RedOrb(const Item_RedOrb& _Other) = delete;
	Item_RedOrb(Item_RedOrb&& _Other) noexcept = delete;
	Item_RedOrb& operator=(const Item_RedOrb& _Other) = delete;
	Item_RedOrb& operator=(Item_RedOrb&& _Other) noexcept = delete;

	void SetWait();
	void Take(GameEngineTransform* _Transform);
protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	GameEngineFSM FSM;
	std::shared_ptr<class FXSystem> Effect = nullptr;
	std::shared_ptr<class GameEngineCollision> Col = nullptr;

	GameEngineTransform* Player;
	enum RedOrbState
	{
		RedOrb_Wait,	// ���� �� ���
		RedOrb_Bursts,	// ���� �׾ Ƣ���
		RedOrb_Bound,	// ���� Ʀ
		RedOrb_Idle,	// ���
		RedOrb_SuckIn,	// �÷��̾ ���
		RedOrb_Disappear// �����
	};
	GameEngineSoundPlayer GetSound;
	float4 Force = float4::ZERO;
	float4 StartPos = float4::ZERO;
	float4 GroundPos = float4::ZERO;
	float Gravity = 0.0f;
	float FloorHeight = 0.0f;
	bool IsTake = false;
	int BoundCount = 0;
};

