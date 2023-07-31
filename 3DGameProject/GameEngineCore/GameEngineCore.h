#pragma once
#include <map>
#include <string>
#include <memory>
#include <Windows.h>
#include <functional>
#include <string_view>
#include <typeinfo>
#include <GameEngineBase/GameEngineThreadJobQueue.h>

#pragma comment(lib, "GameEngineBase.lib")
#pragma comment(lib, "GameEnginePlatform.lib")

#include <GameEngineBase\GameEngineString.h>
#include <GameEngineBase\GameEngineDebug.h>

#include "GameEngineLevel.h"

// ���� :
class GameEngineCore
{
	friend class GameEngineTexture;
	friend class GameEngineSprite;

public:
	// constrcuter destructer
	GameEngineCore();
	~GameEngineCore();

	// delete Function
	GameEngineCore(const GameEngineCore& _Other) = delete;
	GameEngineCore(GameEngineCore&& _Other) noexcept = delete;
	GameEngineCore& operator=(const GameEngineCore& _Other) = delete;
	GameEngineCore& operator=(GameEngineCore&& _Other) noexcept = delete;

	static void Start(HINSTANCE _instance, std::function<void()> _Start, std::function<void()> _End, float4 _Pos = {0, 0}, float4 _Size = { 1280, 720 });

	template<typename LevelType>
	static std::shared_ptr<LevelType> CreateLevel(const std::string_view& _Name = "")
	{
		std::shared_ptr<GameEngineLevel> NewLevel =  std::make_shared<LevelType>();

		std::string Name = _Name.data();

		if (_Name == "")
		{
			const type_info& Info = typeid(LevelType);
			Name = Info.name();
			Name.replace(0, 6, "");
		}

		Name = GameEngineString::ToUpper(Name);

		if (LevelMap.end() != LevelMap.find(Name))
		{
			MsgAssert("���� �̸��� ������ 2�� ������� �����ϴ�.");
		}

		LevelInit(NewLevel);

		LevelMap.insert(std::make_pair(Name, NewLevel));

		return std::dynamic_pointer_cast<LevelType>(NewLevel);
	}
	static void ChangeLevel(const std::string_view& _Name);

	static std::shared_ptr<GameEngineLevel> GetCurLevel() 
	{
		return MainLevel;
	}

	static GameEngineThreadJobQueue JobQueue;

protected:

private:
	static std::shared_ptr<class GameEngineLevel> CurLoadLevel;

	static void CoreResourcesInit();
	static void CoreResourcesEnd();

	static void LevelInit(std::shared_ptr<GameEngineLevel> _Level);

	static void EngineStart(std::function<void()> __ContentsStart);
	static void EngineUpdate();
	static void EngineEnd(std::function<void()> __ContentsEnd);

	static std::map<std::string, std::shared_ptr<GameEngineLevel>> LevelMap;
	static std::shared_ptr<GameEngineLevel> MainLevel;
	static std::shared_ptr<GameEngineLevel> NextLevel;

};

