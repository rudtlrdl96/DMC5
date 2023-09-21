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
};

// ���� : �ش� �������� Effect ���� ������ �Դϴ� Effect_2D �Ǵ� Effect_3D ��Ƽ������ ����ؾ� �մϴ�.
class EffectFBXRenderer : public GameEngineFBXRenderer
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
	EffectFBXRenderer();
	~EffectFBXRenderer();

	// delete Function
	EffectFBXRenderer(const EffectFBXRenderer& _Other) = delete;
	EffectFBXRenderer(EffectFBXRenderer&& _Other) noexcept = delete;
	EffectFBXRenderer& operator=(const EffectFBXRenderer& _Other) = delete;
	EffectFBXRenderer& operator=(EffectFBXRenderer&& _Other) noexcept = delete;

	// �����¿� �������� Clip �մϴ�.
	EffectData EffectOption = EffectData();

	// Engine Rect �Ž��� �������� �ʱ�ȭ �մϴ�.
	void RectInit(const std::string_view& _MaterialName);

	// FBX �Ž��� �������� �ʱ�ȭ �մϴ�.
	void SetFBXMesh(const std::string& _Name, std::string _Material) override;
	void SetFBXMesh(const std::string& _Name, const std::vector<std::vector<std::string>>& _Materials) override;

	void SetFBXMesh(const std::string& _Name, std::string _Material, size_t MeshIndex)  override;
	void SetFBXMesh(const std::string& _Name, std::vector<std::string> _Material, size_t MeshIndex) override;

	// Animation
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

	void SetAnimationUpdateEvent(const std::string_view& _AnimationName, size_t _Frame, std::function<void()> _Event);
	void SetAnimationStartEvent(const std::string_view& _AnimationName, size_t _Frame, std::function<void()> _Event);

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	EffectVertextData VertexOption = EffectVertextData();

	std::map<std::string, std::shared_ptr<AnimationInfo>> Animations;
	std::shared_ptr<AnimationInfo> CurAnimation;
	std::shared_ptr<GameEngineSprite> Sprite = nullptr;
	std::shared_ptr<GameEngineTexture> CurTexture;

	size_t Frame = -1;
	float ScaleRatio = 1.0f;

	void CustomOptionSetting();
	void DrawEditor() override;

};

