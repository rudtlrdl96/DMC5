#pragma once

// ���� :
class PlayerTestLevel : public GameEngineLevel
{
public:
	// constrcuter destructer
	PlayerTestLevel();
	~PlayerTestLevel();

	// delete Function
	PlayerTestLevel(const PlayerTestLevel& _Other) = delete;
	PlayerTestLevel(PlayerTestLevel&& _Other) noexcept = delete;
	PlayerTestLevel& operator=(const PlayerTestLevel& _Other) = delete;
	PlayerTestLevel& operator=(PlayerTestLevel&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render() {};

	void LevelChangeStart() override;
	void LevelChangeEnd() override;

private:
	bool IsMessage = false;
	std::shared_ptr<class AnimationToolWindow> AnimWindow = nullptr;
	std::shared_ptr<class ObjectWindow> ObjWindow = nullptr;

	std::shared_ptr<class PlayerActor_Nero> Nero;
	std::shared_ptr<class PlayerActor_Vergil> Vergil;
	std::shared_ptr<class GameEngineLight> Light;
	std::shared_ptr<class Plane> Flat;
};

