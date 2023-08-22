#pragma once
#include "BaseLevel.h"

// Ό³Έν :
class MainLevel : public BaseLevel
{
public:
	// constrcuter destructer
	MainLevel();
	~MainLevel();

	// delete Function
	MainLevel(const MainLevel& _Other) = delete;
	MainLevel(MainLevel&& _Other) noexcept = delete;
	MainLevel& operator=(const MainLevel& _Other) = delete;
	MainLevel& operator=(MainLevel&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render() {};

	void LevelChangeStart() override;
	void LevelChangeEnd() override;

private:
	bool IsMessage = false;

};

