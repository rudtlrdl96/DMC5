#pragma once

enum class GameEngineOptionValue
{
	None,
	Low,
	High
};

// 설명 :
class GameEngineOption
{
public:
	// 옵션 목록을 생성합니다 만약 이미 옵션이 존재할 경우 BaseOption 데이터를 무시합니다.
	static void CreateOption(const std::string_view& _OptionName, GameEngineOptionValue _BaseOption);

	// 해당 옵션이 존재하는지 반환합니다.
	static bool IsVaild(const std::string_view& _OptionName);

	// 옵션을 변경합니다 만약 옵션이 존재하지 않을 경우 false를 반환합니다.
	static bool ChangeOption(const std::string_view& _OptionName, GameEngineOptionValue _BaseOption);

	// 옵션을 받아옵니다
	static GameEngineOptionValue GetOption(const std::string_view& _OptionName);


private:
	static std::map<std::string, GameEngineOptionValue> OptionValues;

protected:

private:
	// constrcuter destructer
	GameEngineOption();
	~GameEngineOption();

	// delete Function
	GameEngineOption(const GameEngineOption& _Other) = delete;
	GameEngineOption(GameEngineOption&& _Other) noexcept = delete;
	GameEngineOption& operator=(const GameEngineOption& _Other) = delete;
	GameEngineOption& operator=(GameEngineOption&& _Other) noexcept = delete;
};