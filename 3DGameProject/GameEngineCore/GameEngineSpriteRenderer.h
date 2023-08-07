#pragma once
#include "GameEngineRenderer.h"
#include "GameEngineSprite.h"
#include "EngineContentRenderingStruct.h"
#include <map>

class AnimationStartEvent
{
public:
	bool IsEvent = false;
	std::function<void()> Function;
};

class AnimationInfo : public std::enable_shared_from_this<AnimationInfo>
{
	friend class GameEngineSpriteRenderer;

private:
	GameEngineSpriteRenderer* Parent = nullptr;

	std::shared_ptr<GameEngineSprite> Sprite;

	bool IsEndValue = false;

	void Reset();

	void Update(float _DeltaTime);

	const SpriteInfo& CurSpriteInfo();

	inline void PauseOn()
	{
		IsPauseValue = true;
	}

	inline void PauseOff()
	{
		IsPauseValue = false;
	}

public:
	size_t CurFrame = 0;
	size_t StartFrame = -1;
	size_t EndFrame = -1;
	float CurTime = 0.0f;
	float Inter = 0.1f;
	bool Loop = true;
	bool ScaleToTexture = false;
	bool IsPauseValue = false;
	std::vector<size_t> FrameIndex = std::vector<size_t>();
	std::vector<float> FrameTime = std::vector<float>();

	std::map<size_t, std::function<void()>> UpdateEventFunction;
	std::map<size_t, AnimationStartEvent> StartEventFunction;

	bool IsEnd();
};


class AnimationParameter
{
public:
	std::string_view AnimationName = "";
	std::string_view SpriteName = "";
	size_t Start = static_cast<size_t>(-1);
	size_t End = static_cast<size_t>(-1);
	float FrameInter = 0.1f;
	bool Loop = true;
	bool ScaleToTexture = false;
	std::vector<size_t> FrameIndex = std::vector<size_t>();
	std::vector<float> FrameTime = std::vector<float>();
};

enum class ClipXDir
{
	Left,
	Right,
};

enum class ClipYDir
{
	Top,
	Bot,
};


// 설명 :
class GameEngineSpriteRenderer : public GameEngineRenderer
{
public:
	// constrcuter destructer
	GameEngineSpriteRenderer();
	~GameEngineSpriteRenderer();

	// delete Function
	GameEngineSpriteRenderer(const GameEngineSpriteRenderer& _Other) = delete;
	GameEngineSpriteRenderer(GameEngineSpriteRenderer&& _Other) noexcept = delete;
	GameEngineSpriteRenderer& operator=(const GameEngineSpriteRenderer& _Other) = delete;
	GameEngineSpriteRenderer& operator=(GameEngineSpriteRenderer&& _Other) noexcept = delete;

	void SetScaleToTexture(const std::string_view& _Name);

	void SetTexture(const std::string_view& _Name);

	void SetScaleRatio(float _Ratio)
	{
		ScaleRatio = _Ratio;
	}

	void SetFlipX();
	void SetFlipY();

	std::shared_ptr<AnimationInfo> FindAnimation(const std::string_view& _Name);

	std::shared_ptr<AnimationInfo> CreateAnimation(const AnimationParameter& _Paramter);

	void ChangeAnimation(const std::string_view& _Name, bool _Force, size_t _Frame = -1)
	{
		ChangeAnimation(_Name, _Frame, _Force);
	}

	void ChangeAnimation(const std::string_view& _Name, size_t _Frame = -1, bool _Force = true);

	bool IsAnimationEnd()
	{
		return CurAnimation->IsEnd();
	}

	size_t GetCurrentFrame()
	{
		return CurAnimation->FrameIndex[CurAnimation->CurFrame];
	}

	float4 GetAtlasData()
	{
		return AtlasData;
	}

	// 내 눈에 보이는 이미지에서 0.1;
	void ImageClippingX(float _Ratio, ClipXDir _Dir);
	void ImageClippingY(float _Ratio, ClipYDir _Dir);

	inline float GetScaleRatio() const
	{
		return ScaleRatio;
	}

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

	ColorOption ColorOptionValue;


	void SetAnimationUpdateEvent(const std::string_view& _AnimationName, size_t _Frame, std::function<void()> _Event);

	void SetAnimationStartEvent(const std::string_view& _AnimationName, size_t _Frame, std::function<void()> _Event);

	std::string GetTexName();

	void SetRenderEndCallBack(std::function<void(GameEngineRenderer*)> _CallBack)
	{
		RenderEndCallBack = _CallBack;
	}


protected:
	void SpriteRenderInit();
	float4 AtlasData;
	float4 Clip = float4::ONE;
	float4 Flip = float4::ZERO;

	std::function<void(GameEngineRenderer*)> RenderEndCallBack = nullptr;

private:
	void Update(float _Delta) override;

	void Render(float _Delta) override;

	std::map<std::string, std::shared_ptr<AnimationInfo>> Animations;

	std::shared_ptr<AnimationInfo> CurAnimation;

	std::shared_ptr<GameEngineSprite> Sprite = nullptr;
	size_t Frame = -1;

	std::shared_ptr<GameEngineTexture> CurTexture;

	float ScaleRatio = 1.0f;

	void Start() override;
};

