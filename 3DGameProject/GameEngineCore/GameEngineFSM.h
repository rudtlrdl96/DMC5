#pragma once
#include <functional>
#include <map>

class StateParameter 
{
public:
	int StateValue;
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
		int StateValue;
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

	void ChangeFunction(int _StateValue, std::function<void()> Start,
	std::function<void(float _Delta)> Update,
	std::function<void()> End
	);

	void ChangeState(int _StateValue);

	void Update(float _DeltaTime);

	inline bool IsValid() const
	{
		return CurState != nullptr;
	}

protected:
	GameEngineFSM::State* FindState(int _State);

private:
	std::map<int, State> AllState;
	State* CurState = nullptr;

};

