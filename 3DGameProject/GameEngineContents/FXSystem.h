#pragma once
#include "FXData.h"
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

	static void EffectTextureLoad();

	void SetFX(std::shared_ptr<class FXData> _FX);
	void SetFX(const std::string_view& _Name);

	void CreateFX(std::shared_ptr<class FXData> _FX);
	void ChangeFX(const std::string_view& _Name);
	void ChangeFX(std::shared_ptr<class FXData> _FX);

	void PlayFX(const std::string_view& _Name)
	{
		ChangeFX(_Name);
		Play();
	}

	void SetCurFrame(const int _Frame)
	{
		CurFrame = _Frame;
	}

	std::shared_ptr<class FXData> GetFX()
	{
		return CurFX;
	}

	void SetDistortionTextureName(const std::string_view& _Name)
	{
		DistortionTextureName = _Name;
	}
	
	void Play();

	float CurFrameTime = 0.0f;
	float Inter = 0.0166f;
	float TimeScale = 1.0f;

	int CurFrame = 0;
	int StartFrame = -1;
	int EndFrame = -1;

	bool Loop = false;
	bool Pause = false;

	bool IsUI = false;
protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void FXSetting();
	void RenderOff();
private:
	std::map<std::string , std::shared_ptr<class EffectRenderer>> FXRenders;
	std::shared_ptr<FXData> CurFX = nullptr;
	std::string_view DistortionTextureName = "DistortionSample_00.jpg";
};

