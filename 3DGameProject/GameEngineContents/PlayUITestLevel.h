#pragma once
#include "BaseLevel.h"

// Ό³Έν :
class PlayUITestLevel : public BaseLevel
{
public:
	// constrcuter destructer
	PlayUITestLevel();
	~PlayUITestLevel();

	// delete Function
	PlayUITestLevel(const PlayUITestLevel& _Other) = delete;
	PlayUITestLevel(PlayUITestLevel&& _Other) noexcept = delete;
	PlayUITestLevel& operator=(const PlayUITestLevel& _Other) = delete;
	PlayUITestLevel& operator=(PlayUITestLevel&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render() {};

	void LevelChangeStart() override;
	void LevelChangeEnd() override;

private:

};

