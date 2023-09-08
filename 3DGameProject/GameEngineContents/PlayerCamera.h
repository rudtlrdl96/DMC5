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
	void SetTargetTranform(GameEngineTransform* _Transform)
	{
		TargetTransform = _Transform;
	}
	GameEngineTransform* GetCameraTarget()
	{
		return CameraTarget;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void TargetCheck(float _DeltaTime);
	void WallCheck();
	
private:
	GameEngineTransform* PlayerTransform = nullptr;
	GameEngineTransform* TargetTransform = nullptr;
	GameEngineTransform* CameraArm = nullptr;
	GameEngineTransform* CameraTarget = nullptr;
	GameEngineTransform* CameraTransform = nullptr;
	float4 CameraTargetPos;
	float4 CameraDistance = { 0, 100, -300 };
	float4 CameraFarDistance = { 0, 0, -500 };
	
	float CameraRotYSpeed = 150.0f;
	float CameraRotXSpeed = 80.0f;

	float MinDistance = 400.0f;
	float TrackingSpeed = 5.0f;
	float LockOnTrackingSpeed = 10.0f;

	float FastTrackingDistance = 400.0f;
	float FastTrackingSpeed = 20.0f;
};

