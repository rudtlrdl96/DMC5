#pragma once
#include <GameEngineCore/GameEngineActor.h>


// 설명 :
class PickableActor : public GameEngineActor
{
public:

	/// CreateActor<PickableActor> 후에 무조건 호출해줘야함

	// 유저 메쉬 세팅할 PickableActor 세팅
	void SetStaticMesh(const std::string& _FBX, const std::string& _Texture = "");

	// Collision만 있는 PickableActor 세팅, 사이즈만 변경하고 싶을 때도 호출 가능
	void SetCollisionOnly(const float4& _Scale);

	/// ~ CreateActor<PickableActor> 후에 무조건 호출해줘야함

	// Axis 세팅
	void SetAxisMove(float4 _Color, float4 _Scale, float4 _MoveDir);
	void SetAxisRot(float4 _Color, float4 _Scale);


	bool IsAxis()
	{
		return IsAxis_;
	}

	float4 GetMoveDir()
	{
		return MoveDir_;
	}

public:
	static std::shared_ptr<GameEngineCollision> CurPicking_Collision;

	// constrcuter destructer
	PickableActor();
	~PickableActor();

	// delete Function
	PickableActor(const PickableActor& _Other) = delete;
	PickableActor(PickableActor&& _Other) noexcept = delete;
	PickableActor& operator=(const PickableActor& _Other) = delete;
	PickableActor& operator=(PickableActor&& _Other) noexcept = delete;

protected:
	/////////////////////////////////////////////////////////////////////
	//	엑터 피킹
	/////////////////////////////////////////////////////////////////////

		// 피킹 레이저와 닿았는지 여부 체크
	void CheckPickingRay();
	void CheckSelected();

	// 맵 에딧용 콜리전, 캐릭용 콜리전은 별도로 생각해야함
	std::shared_ptr<GameEngineCollision> Collision_Picking_;
	std::shared_ptr<class GameEngineFBXStaticRenderer> FBXRenderer_;

	std::shared_ptr<class GameEngineTextureRenderer> CollisionRenderer_;
	float4 ResultColor_;

	void Start() override;
	void Update(float _DeltaTime) override;

private:
	// Axis 용
	bool IsAxis_;
	float4 MoveDir_;

	// 유저메쉬 없이 콜리전만 생성하고 싶다(발판, 세이브포인트, 골 지점)


};

