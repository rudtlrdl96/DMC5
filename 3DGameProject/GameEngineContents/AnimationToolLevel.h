#pragma once

// Ό³Έν :
class AnimationToolLevel : public GameEngineLevel
{
public:
	// constrcuter destructer
	AnimationToolLevel();
	~AnimationToolLevel();

	// delete Function
	AnimationToolLevel(const AnimationToolLevel& _Other) = delete;
	AnimationToolLevel(AnimationToolLevel&& _Other) noexcept = delete;
	AnimationToolLevel& operator=(const AnimationToolLevel& _Other) = delete;
	AnimationToolLevel& operator=(AnimationToolLevel&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render() {};

	void LevelChangeStart() override;
	void LevelChangeEnd() override;

private:
	bool IsMessage = false;
	std::shared_ptr<class AnimationToolWindow> AnimWindow = nullptr;

};

