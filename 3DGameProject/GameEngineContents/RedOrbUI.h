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
		PrevOrb = RedOrbValue;
		RedOrbValue += _Value;
	}
	static int GetRedOrb()
	{
		return RedOrbValue;
	}
	static void SubRedOrb(int _Value)
	{
		PrevOrb = RedOrbValue;
		RedOrbValue -= _Value;
	}
protected:
	void Start() override;
	void Update(float _Delta) override;
private:

	std::shared_ptr<class GameEngineUIRenderer> RedOrbRender = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> RedOrbFont = nullptr;
	float4 Scale = { 43.0f,60.0f,0.0f };
	static int PrevOrb;
	static int RedOrbValue;
	float AddTime = 0.0f;
	float RedOrbNum = 0.0f;
};

