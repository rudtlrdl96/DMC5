#pragma once

// Ό³Έν :
class PlayerController : public GameEngineComponent
{
public:
	// constrcuter destructer
	PlayerController();
	~PlayerController();

	// delete Function
	PlayerController(const PlayerController& _Other) = delete;
	PlayerController(PlayerController&& _Other) noexcept = delete;
	PlayerController& operator=(const PlayerController& _Other) = delete;
	PlayerController& operator=(PlayerController&& _Other) noexcept = delete;

	float4 GetMoveVector()
	{
		return MoveAxis;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	float4 MoveAxis;
};

