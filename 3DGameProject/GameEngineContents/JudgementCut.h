#pragma once
#include <GameEngineCore/GameEngineRenderTarget.h>

// ���� :

class JudgementCut : public GameEnginePostProcess
{
	class JudgementBuffer
	{
	public:
		float4 Lines[32] = {};
		int LineCount = 0;
		float LineThickness = 1.5f;
		float LineInter = 0.5f;
	};

public:
	// constrcuter destructer
	JudgementCut();
	~JudgementCut();

	// delete Function
	JudgementCut(const JudgementCut& _Other) = delete;
	JudgementCut(JudgementCut&& _Other) noexcept = delete;
	JudgementCut& operator=(const JudgementCut& _Other) = delete;
	JudgementCut& operator=(JudgementCut&& _Other) noexcept = delete;

	// ȿ���� ���α��⸦ �����մϴ�. �⺻ = 1.5f
	inline void SetLineThickness(float _Value)
	{
		ShaderBuffer.LineThickness = _Value;
	}

	// ȿ���� ���ǵ带 �����մϴ�. �⺻ = 25.0f
	inline void SetSpeed(float _Speed)
	{
		Speed = _Speed;
	}

	// ȿ���� �ִ� ���� �����մϴ�. �⺻ = 5.0f
	inline void SetMaxCut(float _MaxCut)
	{
		MaxCut = _MaxCut;
	}

	void AddLine(float4 _Line);

	void JudgementCutOn();
	void JudgementCutOff();

protected:
	void Start(GameEngineRenderTarget* _Target) override;
	void Effect(GameEngineRenderTarget* _Target, float _DeltaTime) override;

private:
	std::shared_ptr<GameEngineRenderUnit> JudgementUnit = nullptr;

	RenderBaseValue BaseValue = RenderBaseValue();
	JudgementBuffer ShaderBuffer = JudgementBuffer();

	void LevelChangeStart() override
	{
		int a = 0;
	}

	bool IsEffectOn = false;

	float Speed = 25.0f;
	float MaxCut = 5.0f;
};

