#pragma once
#include "BaseLevel.h"
// Ό³Έν :
class MainMenuLevel : public BaseLevel
{
public:
	// constrcuter destructer
	MainMenuLevel();
	~MainMenuLevel();

	// delete Function
	MainMenuLevel(const MainMenuLevel& _Other) = delete;
	MainMenuLevel(MainMenuLevel&& _Other) noexcept = delete;
	MainMenuLevel& operator=(const MainMenuLevel& _Other) = delete;
	MainMenuLevel& operator=(MainMenuLevel&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render() {};

	void LevelChangeStart() override;
	void LevelChangeEnd() override;
private:
	std::shared_ptr<class FadeEffect> FEffect;
	std::shared_ptr<class Menu_VirgilInfo> Menu_VirgilInfoPtr = nullptr;
	std::shared_ptr<class Menu_NeroInfo> Menu_NeroInfoPtr = nullptr;
};

