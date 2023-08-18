#pragma once

// Ό³Έν :
class PlayerCamera : public GameEngineComponent
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

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	std::shared_ptr<GameEngineComponent> CameraTarget = nullptr;
	GameEngineTransform* TargetTransform;
	GameEngineTransform* CameraTransform = nullptr;
};

