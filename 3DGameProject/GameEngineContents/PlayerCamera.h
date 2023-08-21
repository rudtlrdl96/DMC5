#pragma once

// Ό³Έν :
class PlayerCamera : public GameEngineActor
{
public:
	// constrcuter destructer
	PlayerCamera();
	~PlayerCamera();

	// delete Function
	PlayerCamera(const PlayerCamera& _Other) = delete;
	PlayerCamera(PlayerCamera&& _Other) noexcept = delete;
	PlayerCamera& operator=(const PlayerCamera& _Other) = delete;
	PlayerCamera& operator=(PlayerCamera&& _Other) noexcept = delete;

	void SetPlayerTranform(GameEngineTransform* _Transform)
	{
		PlayerTransform = _Transform;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	GameEngineTransform* PlayerTransform;
	GameEngineTransform* CameraArm;
	GameEngineTransform* CameraTarget;
	GameEngineTransform* CameraTransform = nullptr;
};

