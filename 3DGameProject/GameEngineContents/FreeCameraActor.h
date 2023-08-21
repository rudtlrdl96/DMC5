#pragma once
// Ό³Έν :
class FreeCameraActor : public GameEngineActor
{
public:
	// constrcuter destructer
	FreeCameraActor();
	~FreeCameraActor();

	// delete Function
	FreeCameraActor(const FreeCameraActor& _Other) = delete;
	FreeCameraActor(FreeCameraActor&& _Other) noexcept = delete;
	FreeCameraActor& operator=(const FreeCameraActor& _Other) = delete;
	FreeCameraActor& operator=(FreeCameraActor&& _Other) noexcept = delete;

	float MoveSpeed = 500;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	GameEngineTransform* CameraTransform = nullptr;

};

