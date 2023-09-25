#pragma once

// Ό³Έν :
class PhysXTestLevel : public GameEngineLevel
{
public:
	// constrcuter destructer
	PhysXTestLevel();
	~PhysXTestLevel();

	// delete Function
	PhysXTestLevel(const PhysXTestLevel& _Other) = delete;
	PhysXTestLevel(PhysXTestLevel&& _Other) noexcept = delete;
	PhysXTestLevel& operator=(const PhysXTestLevel& _Other) = delete;
	PhysXTestLevel& operator=(PhysXTestLevel&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render() {};

	void LevelChangeStart() override;
	void LevelChangeEnd() override;

private:
	std::shared_ptr<class AnimationToolWindow> AnimWindow = nullptr;
	std::shared_ptr<class ObjectWindow> ObjWindow = nullptr;

};

