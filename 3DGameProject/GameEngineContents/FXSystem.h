#pragma once

// Ό³Έν :
class FXSystem : public GameEngineComponent
{
public:
	// constrcuter destructer
	FXSystem();
	~FXSystem();

	// delete Function
	FXSystem(const FXSystem & _Other) = delete;
	FXSystem(FXSystem && _Other) noexcept = delete;
	FXSystem& operator=(const FXSystem & _Other) = delete;
	FXSystem& operator=(FXSystem && _Other) noexcept = delete;

	void SetFX(const std::string_view& _Name);

	float CurFrameTime = 0.0f;
	float Inter = 0.1f;
	float TimeScale = 1.0f;

	int CurFrame = 0;
	int StartFrame = -1;
	int EndFrame = -1;

	bool Loop = true;
	bool Pause = false;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void FXSetting();
private:
	std::vector<std::shared_ptr<class EffectRenderer>> FXRenders;
	std::shared_ptr<class FXData> CurFX;

};

