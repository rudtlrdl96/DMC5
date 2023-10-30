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
	static int GetRedOrb()
	{
		return RedOrbValue;
	}
	static void SubRedOrb(int _Value)
	{
		RedOrbValue -= _Value;
	}
protected:
	void Start() override;
	void Update(float _Delta) override;
private:
	void AniCreate();
	void UpdateOrb(float _Delta);
	std::shared_ptr<class GameEngineUIRenderer> RedOrbRender = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> RedOrbFont = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> Num_Ani1 = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> Num_Ani2 = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> Num_Ani3 = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> Num_Ani4 = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> Num_Ani5 = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> Num_Ani6 = nullptr;
	float4 Scale = { 43.0f,60.0f,0.0f };
	float4 AniScale = { 33.0f,60.0f,0.0f };

	static int RedOrbValue;
	static bool IsRedOrbAni;
	bool IsOff = false;
	float AniTime = 0.0f;
};

