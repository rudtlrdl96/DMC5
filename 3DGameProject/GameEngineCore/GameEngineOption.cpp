#include "PrecompileHeader.h"
#include "GameEngineOption.h"

std::map<std::string, GameEngineOptionValue> GameEngineOption::OptionValues;

void GameEngineOption::CreateOption(const std::string_view& _OptionName, GameEngineOptionValue _BaseOption)
{
	std::string UpperName = GameEngineString::ToUpper(_OptionName);

	if (true == OptionValues.contains(UpperName))
	{
		return;
	}

	OptionValues[UpperName] = _BaseOption;
}

bool GameEngineOption::IsVaild(const std::string_view& _OptionName)
{
	std::string UpperName = GameEngineString::ToUpper(_OptionName);

	if (true == OptionValues.contains(UpperName))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool GameEngineOption::ChangeOption(const std::string_view& _OptionName, GameEngineOptionValue _BaseOption)
{
	std::string UpperName = GameEngineString::ToUpper(_OptionName);

	if (false == OptionValues.contains(UpperName))
	{
		MsgAssert(std::string("옵션이 존재하지 않습니다 : ") + _OptionName.data());
		return false;
	}

	OptionValues[UpperName] = _BaseOption;
	return true;
}

GameEngineOptionValue GameEngineOption::GetOption(const std::string_view& _OptionName)
{
	std::string UpperName = GameEngineString::ToUpper(_OptionName);

	if (false == OptionValues.contains(UpperName))
	{
		MsgAssert(std::string("옵션이 존재하지 않습니다 : ") + _OptionName.data());
		return GameEngineOptionValue::Low;
	}

	return OptionValues[UpperName];
}

GameEngineOption::GameEngineOption()
{
}

GameEngineOption::~GameEngineOption() 
{
}

