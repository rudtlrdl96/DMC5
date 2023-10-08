#pragma once
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineSpriteRenderer.h>

class EffectData
{
public:
	float ClipStartX = 0;
	float ClipEndX = 1;
	float ClipStartY = 0;
	float ClipEndY = 1;
	float UVX = 0;
	float UVY = 0;
	float4 MulColor = float4::ONE;
	float4 PlusColor = float4::ZERONULL;

	static EffectData Lerp(const EffectData& Start, const EffectData& End, float Ratio)
	{
		EffectData Result;
		Result.ClipStartX = GameEngineMath::Lerp(Start.ClipStartX, End.ClipStartX, Ratio);
		Result.ClipEndX = GameEngineMath::Lerp(Start.ClipEndX, End.ClipEndX, Ratio);
		Result.ClipStartY = GameEngineMath::Lerp(Start.ClipStartY, End.ClipStartY, Ratio);
		Result.ClipEndY = GameEngineMath::Lerp(Start.ClipEndY, End.ClipEndY, Ratio);
		Result.UVX = GameEngineMath::Lerp(Start.UVX, End.UVX, Ratio);
		Result.UVY = GameEngineMath::Lerp(Start.UVY, End.UVY, Ratio);
		Result.MulColor = float4::Lerp(Start.MulColor, End.MulColor, Ratio);
		Result.PlusColor = float4::Lerp(Start.PlusColor, End.PlusColor, Ratio);
		return Result;
	}

	void Write(GameEngineSerializer& _File)
	{
		_File << ClipStartX;
		_File << ClipEndX;
		_File << ClipStartY;
		_File << ClipEndY;
		_File << UVX;
		_File << UVY;
		_File << MulColor;
		_File << PlusColor;
	}

	void Read(GameEngineSerializer& _File)
	{
		_File >> ClipStartX;
		_File >> ClipEndX;
		_File >> ClipStartY;
		_File >> ClipEndY;
		_File >> UVX;
		_File >> UVY;
		_File >> MulColor;
		_File >> PlusColor;
	}
};

class DistortionData
{
public:
	float4 IsDistortion = float4::ZERONULL;
};

// 설명 : 해당 랜더러는 Effect 전용 렌더러 입니다 Effect_2D 또는 Effect_3D 머티리얼을 사용해야 합니다.
class EffectRenderer : public GameEngineFBXRenderer
{
private:
	class EffectVertextData
	{
	public:
		float IsLockRotation = 0.0f;
		float4 FramePos = float4(0, 0);
		float4 FrameScale = float4(1, 1);
		float4 Flip = float4::ZERO;
	};
public:
	// constrcuter destructer
	EffectRenderer();
	~EffectRenderer();

	// delete Function
	EffectRenderer(const EffectRenderer& _Other) = delete;
	EffectRenderer(EffectRenderer&& _Other) noexcept = delete;
	EffectRenderer& operator=(const EffectRenderer& _Other) = delete;
	EffectRenderer& operator=(EffectRenderer&& _Other) noexcept = delete;

	// 이펙트 옵션을 설정합니다.
	EffectData EffectOption = EffectData();

	// Engine Rect 매쉬를 기준으로 초기화 합니다.
	void RectInit(const std::string_view& _MaterialName);
	void SphereInit(const std::string_view& _MaterialName);

	// FBX 매쉬를 기준으로 초기화 합니다.
	void SetFBXMesh(const std::string_view& _Name, const std::string_view& _Material) override;
	void SetFBXMesh(const std::string_view& _Name, const std::vector<std::vector<std::string>>& _Materials) override;

	void SetFBXMesh(const std::string_view& _Name, const std::string_view& _Material, size_t MeshIndex)  override;
	void SetFBXMesh(const std::string_view& _Name, std::vector<std::string> _Material, size_t MeshIndex) override;

	// Animation
	void SetScaleRatio(float _Ratio)
	{
		ScaleRatio = _Ratio;
	}

	void SetFlipX();
	void SetFlipY();

	std::shared_ptr<AnimationInfo> FindAnimation(const std::string_view& _Name);

	std::shared_ptr<AnimationInfo> CreateAnimation(const AnimationParameter& _Paramter);

	void AnimationClear()
	{
		CurAnimation = nullptr;
		VertexOption.FramePos = float4(0, 0);
		VertexOption.FrameScale = float4(1, 1);
	}

	void ChangeAnimation(const std::string_view& _Name, bool _Force, size_t _Frame = -1)
	{
		ChangeAnimation(_Name, _Frame, _Force);
	}

	void ChangeAnimation(const std::string_view& _Name, size_t _Frame = -1, bool _Force = true);
	
	std::shared_ptr<AnimationInfo> GetCurAnimation()
	{
		return CurAnimation;
	}

	bool IsAnimationEnd()
	{
		return CurAnimation->IsEnd();
	}

	size_t GetCurrentFrame()
	{
		return CurAnimation->FrameIndex[CurAnimation->CurFrame];
	}

	inline float GetScaleRatio() const
	{
		return ScaleRatio;
	}

	void SetDistortionTexture(const std::string_view& _TextureName = "");

	void SetSprite(const std::string_view& _SpriteName, size_t _Frame = 0);
	void SetFrame(size_t _Frame);

	void SetAnimPauseOn()
	{
		CurAnimation->PauseOn();
	}

	void SetAnimPauseOff()
	{
		CurAnimation->PauseOff();
	}

	void SetAnimationUpdateEvent(const std::string_view& _AnimationName, size_t _Frame, std::function<void()> _Event);
	void SetAnimationStartEvent(const std::string_view& _AnimationName, size_t _Frame, std::function<void()> _Event);

	// 이펙트가 카메라 정면을 바라보는 기능을 On합니다.
	inline void LockRotation()
	{
		VertexOption.IsLockRotation = 1.0f;
	}

	// 이펙트가 카메라 정면을 바라보는 기능을 Off합니다.
	inline void UnLockRotation()
	{
		VertexOption.IsLockRotation = 0.0f;
	}

	inline bool IsLockRotation() const
	{
		return 0.0f != VertexOption.IsLockRotation;
	}

	void Reset();

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	EffectVertextData VertexOption = EffectVertextData();
	DistortionData DistortionOption = DistortionData();

	std::map<std::string, std::shared_ptr<AnimationInfo>> Animations;
	std::shared_ptr<AnimationInfo> CurAnimation;
	std::shared_ptr<GameEngineSprite> Sprite = nullptr;
	std::shared_ptr<GameEngineTexture> CurTexture;

	size_t Frame = -1;
	float ScaleRatio = 1.0f;

	void CustomOptionSetting();
	void DrawEditor() override;

};

