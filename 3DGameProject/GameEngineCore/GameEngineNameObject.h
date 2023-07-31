#pragma once
#include <string>

// Ό³Έν :
class GameEngineNameObject
{
public:
	// constrcuter destructer
	GameEngineNameObject();
	~GameEngineNameObject();

	// delete Function
	GameEngineNameObject(const GameEngineNameObject& _Other) = delete;
	GameEngineNameObject(GameEngineNameObject&& _Other) noexcept = delete;
	GameEngineNameObject& operator=(const GameEngineNameObject& _Other) = delete;
	GameEngineNameObject& operator=(GameEngineNameObject&& _Other) noexcept = delete;

	void SetName(const std::string_view& _Name)
	{
		Name = _Name;
	}

	std::string_view GetName()
	{
		return Name.c_str();
	}

	std::string GetNameToString()
	{
		return Name;
	}


protected:

private:
	std::string Name = "";

};

