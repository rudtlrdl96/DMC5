#pragma once

// 설명 :
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

	void SetCameraCutscene(const float4& _StartPos, const float4& _EndPos, const float4& _StartRot, const float4& _EndRot, float _Time);

	static void Shake(float _Power, float _Time, float _FadeIn = 0.0f, float _FadeOut = 0.0f);
	static void ShakeLight()
	{
		Shake(2.5f, 0.3f, 0.0f, 0.2f);
	}
	static void ShakeMiddle()
	{
		Shake(3.0f, 0.4f, 0.1f, 0.2f);
	}
	static void ShakeHeavy()
	{
		Shake(5.0f, 0.8f, 0.1f, 0.5f);
	}

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

	void SetBossCam(GameEngineTransform* _BossTransform)
	{
		BossTransform = _BossTransform;

		MinDistance = 380.0f;
		CameraDistance.z = -200 ;
	}

	void SetRotation(const float4& _Rot);

	void SetLookTarget(const float4& TargetPos);

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void CameraControll(float _DeltaTime);
	void TargetCheck(float _DeltaTime);
	void WallCheck();
	void ShakeUpdate(float _DeltaTime);
	
	void CutSceneUpdate(float _DeltaTime);

private:
	float4 CameraCutsceneStartPos;
	float4 CameraCutsceneEndPos;
	float4 CameraCutsceneStartRot;
	float4 CameraCutsceneEndRot;
	float CameraCutsceneSpeed;
	float CameraCutsceneRatio;
	bool IsCameraCutscene;
	
	std::shared_ptr<GameEngineActor> FreeCamActor;
	GameEngineTransform* PlayerTransform = nullptr;
	GameEngineTransform* TargetTransform = nullptr;
	GameEngineTransform* CameraArm = nullptr;
	GameEngineTransform* CameraTarget = nullptr;
	GameEngineTransform* CameraTransform = nullptr;
	GameEngineTransform* BossTransform = nullptr;
	float4 CameraTargetPos;
	float4 CameraDistance = { 0, 100, -300 };
	float4 CameraFarDistance = { 0, 0, -500 };
	float4 CameraRot;
	float4 BeforeMouseRot;
	
	float MouseAcceleration = 0.8f;	// 마우스 가속도
	float MouseSensitivity = 0.25f;		// 마우스 감도
	float CameraRotBraking = 0.3f;		// 카메라 제동력
	float CameraRotAcceleration = 5.0f;	// 카메라 회전 가속도
	float CameraRotYSpeed = 120.0f;		// 카메라 y축 회전 속도
	float CameraRotXSpeed = 80.0f;		// 카메라 x축 회전 속도

	float DistanceMul = 0.68f;
	float MinDistance = 260.0f;
	float MaxDistance = 1000.0f;
	float TrackingSpeed = 5.0f;
	float LockOnTrackingSpeed = 10.0f;

	float FastTrackingDistance = 120.0f;
	float FastTrackingSpeed = 0.02f;

	bool IsMouseControll = false;
	bool FirstFrame = true;

	bool IsFreeCamera = false;

	void DrawEditor() override;

};

