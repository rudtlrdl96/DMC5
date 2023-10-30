#pragma once
#include <GameEngineCore/GameEngineActor.h>
// Ό³Έν :
class RedOrbUI : public GameEngineActor
{
public:
	// constrcuter destructer
	RedOrbUI();
	~RedOrbUI();

	// delete Function
	RedOrbUI(const RedOrbUI& _Other) = delete;
	RedOrbUI(RedOrbUI&& _Other) noexcept = delete;
	RedOrbUI& operator=(const RedOrbUI& _Other) = delete;
	RedOrbUI& operator=(RedOrbUI&& _Other) noexcept = delete;
	static void AddRedOrb(int _Value)
	{
		RedOrbValue += _Value;
	}
protected:
	void Start() override;
	void Update(float _Delta) override;
private:
	std::shared_ptr<class GameEngineUIRenderer> RedOrbRender = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> RedOrbFont = nullptr;
	static int RedOrbValue;

};

