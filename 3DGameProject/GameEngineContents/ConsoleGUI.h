#pragma once
#include <GameEngineCore/GameEngineGUI.h>

// Ό³Έν :
class ConsoleGUI : public GameEngineGUIWindow
{
public:
	// constrcuter destructer
	ConsoleGUI();
	~ConsoleGUI();

	// delete Function
	ConsoleGUI(const ConsoleGUI& _Other) = delete;
	ConsoleGUI(ConsoleGUI&& _Other) noexcept = delete;
	ConsoleGUI& operator=(const ConsoleGUI& _Other) = delete;
	ConsoleGUI& operator=(ConsoleGUI&& _Other) noexcept = delete;

protected:
	void Start() override;
	void OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime) override;

private:

	int PrintLogOrder = 0;

	const char* LogTypeNames[(int)LogOrder::Count] = {0};
	char MoveLevelName[255] = {0};

};

