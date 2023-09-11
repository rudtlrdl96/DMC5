#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include "UIButton.h"
// Ό³Έν :
class Title_SelectScreen : public GameEngineActor
{
public:
	// constrcuter destructer
	Title_SelectScreen();
	~Title_SelectScreen();

	// delete Function
	Title_SelectScreen(const Title_SelectScreen& _Other) = delete;
	Title_SelectScreen(Title_SelectScreen&& _Other) noexcept = delete;
	Title_SelectScreen& operator=(const Title_SelectScreen& _Other) = delete;
	Title_SelectScreen& operator=(Title_SelectScreen&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
private:
	void SetFontText();
	void SetButtonIndex();
	std::shared_ptr<UIButton> ContinueButton = nullptr;
	std::shared_ptr<UIButton> OptionButton = nullptr;
	std::shared_ptr<UIButton> ExitButton = nullptr;

	bool ContinueValue = false;
	int SelectValue = 0;
	float EnterTime = 0.0f;

};

