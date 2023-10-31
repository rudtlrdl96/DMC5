#pragma once
#include "ContentsEnum.h"
#include "FieldMapObject.h"
// 설명 :
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
		RedOrb_Wait,	// 생성 전 대기
		RedOrb_Bursts,	// 적이 죽어서 튀어나옴
		RedOrb_Bound,	// 통통 튐
		RedOrb_Idle,	// 대기
		RedOrb_SuckIn,	// 플레이어가 흡수
		RedOrb_Disappear// 사라짐
	};

	float4 Force = float4::ZERO;
	float4 StartPos = float4::ZERO;
	float4 GroundPos = float4::ZERO;
	float Gravity = 0.0f;
	float FloorHeight = 0.0f;
	bool IsTake = false;
	int BoundCount = 0;
};

