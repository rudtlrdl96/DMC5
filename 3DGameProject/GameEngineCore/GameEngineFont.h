#pragma once
#include "GameEngineResource.h"

#include "ThirdParty/FW1Font/Inc/FW1FontWrapper.h"

#pragma comment(lib, "FW1FontWrapper.lib")

// Ό³Έν :
class GameEngineFont : public GameEngineResource<GameEngineFont>
{
	friend class FontFactoryCreator;
	static IFW1Factory* Factory;

public:
	// constrcuter destructer
	GameEngineFont();
	~GameEngineFont();

	// delete Function
	GameEngineFont(const GameEngineFont& _Other) = delete;
	GameEngineFont(GameEngineFont&& _Other) noexcept = delete;
	GameEngineFont& operator=(const GameEngineFont& _Other) = delete;
	GameEngineFont& operator=(GameEngineFont&& _Other) noexcept = delete;

	static std::shared_ptr<GameEngineFont> Load(const std::string_view& _FontName)
	{
		std::shared_ptr<GameEngineFont> Res = GameEngineFont::Create(_FontName);
		Res->LoadFont(_FontName);
		return Res;
	}

	void FontDraw(const std::string_view& _Text, const float4& _Pos, float _FontScale, const float4& _Color, FW1_TEXT_FLAG _FwTextPlag);

protected:

private:
	IFW1FontWrapper* Font;
	void LoadFont(const std::string_view& _Path);
};

