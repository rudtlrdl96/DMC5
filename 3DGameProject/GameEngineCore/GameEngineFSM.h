#pragma once
#include <functional>
#include <map>

class StateParameter 
{
public:
	std::string_view Name;
	std::function<void()> Start;
	std::function<void(float _Delta)> Update;
	std::function<void()> End;
};

// Ό³Έν :
class GameEngineFSM
{
public:
	class State 
	{
	public:
		std::string Name;
		float CurStateTime;
		std::function<void()> Start;
		std::function<void(float _Delta)> Update;
		std::function<void()> End;
	};

public:
	// constrcuter destructer
	GameEngineFSM();
	~GameEngineFSM();

	// delete Function
	GameEngineFSM(const GameEngineFSM& _Other) = delete;
	GameEngineFSM(GameEngineFSM&& _Other) noexcept = delete;
	GameEngineFSM& operator=(const GameEngineFSM& _Other) = delete;
	GameEngineFSM& operator=(GameEngineFSM&& _Other) noexcept = delete;

	void CreateState(const StateParameter& _StateFunction);

	void ChangeFunction(const std::string_view& _Name, std::function<void()> Start,
	std::function<void(float _Delta)> Update,
	std::function<void()> End
	);

	void ChangeState(const std::string_view& _Name);

	void Update(float _DeltaTime);

	inline bool IsValid() const
	{
		return CurState != nullptr;
	}

protected:
	GameEngineFSM::State* FindState(const std::string_view& _Name);

private:
	std::map<std::string, State> AllState;
	State* CurState = nullptr;

};

