#pragma once
#include <GameEngineCore/GameEngineActor.h>
// Ό³Έν :
class Menu_SelectScreen :public GameEngineActor
{
public:
	// constrcuter destructer
	Menu_SelectScreen();
	~Menu_SelectScreen();

	// delete Function
	Menu_SelectScreen(const Menu_SelectScreen& _Other) = delete;
	Menu_SelectScreen(Menu_SelectScreen&& _Other) noexcept = delete;
	Menu_SelectScreen& operator=(const Menu_SelectScreen& _Other) = delete;
	Menu_SelectScreen& operator=(Menu_SelectScreen&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _Delta) override;
private:
	void SetFontText();
	void SetButtonIndex();

	std::shared_ptr<class UIButton> MissionButton= nullptr;
	std::shared_ptr<class UIButton> CustomizeButton = nullptr;
	std::shared_ptr<class UIButton> PrevButton = nullptr;
	int SelectValue = 0;
};

