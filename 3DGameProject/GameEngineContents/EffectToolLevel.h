#pragma once

// Ό³Έν :
class EffectToolLevel : public GameEngineLevel
{
public:
	// constrcuter destructer
	EffectToolLevel();
	~EffectToolLevel();

	// delete Function
	EffectToolLevel(const EffectToolLevel& _Other) = delete;
	EffectToolLevel(EffectToolLevel&& _Other) noexcept = delete;
	EffectToolLevel& operator=(const EffectToolLevel& _Other) = delete;
	EffectToolLevel& operator=(EffectToolLevel&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render() {};

	void LevelChangeStart() override;
	void LevelChangeEnd() override;

private:
	std::shared_ptr<class EffectToolWindow> ToolWindow = nullptr;

};

