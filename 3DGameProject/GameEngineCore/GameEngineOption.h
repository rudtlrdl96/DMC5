#pragma once

enum class GameEngineOptionValue
{
	None,
	Low,
	High
};

// ���� :
class GameEngineOption
{
public:
	// �ɼ� ����� �����մϴ� ���� �̹� �ɼ��� ������ ��� BaseOption �����͸� �����մϴ�.
	static void CreateOption(const std::string_view& _OptionName, GameEngineOptionValue _BaseOption);

	// �ش� �ɼ��� �����ϴ��� ��ȯ�մϴ�.
	static bool IsVaild(const std::string_view& _OptionName);

	// �ɼ��� �����մϴ� ���� �ɼ��� �������� ���� ��� false�� ��ȯ�մϴ�.
	static bool ChangeOption(const std::string_view& _OptionName, GameEngineOptionValue _BaseOption);

	// �ɼ��� �޾ƿɴϴ�
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