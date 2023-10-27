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

// 설명 :
class GameEngineCore
{
	friend class GameEngineTexture;
	friend class GameEngineSprite;

	class CaptureData
	{
	public:
		std::string CaptureName = "";
		std::shared_ptr<GameEngineRenderTarget> CaptureTarget = nullptr;
		size_t TexIndex = 0;
		float4 Scale = float4::ZERO;
	};

public:
	// constrcuter destructer
	GameEngineCore();
	~GameEngineCore();

	// delete Function
	GameEngineCore(const GameEngineCore& _Other) = delete;
	GameEngineCore(GameEngineCore&& _Other) noexcept = delete;
	GameEngineCore& operator=(const GameEngineCore& _Other) = delete;
	GameEngineCore& operator=(GameEngineCore&& _Other) noexcept = delete;

	static void Start(HINSTANCE _instance, std::function<void()> _Start, std::function<void()> _End, float4 _Pos = { 0, 0 }, float4 _Size = { 1600, 900 }, bool _IsFullScreen = false);

	template<typename LevelType>
	static std::shared_ptr<LevelType> CreateLevel(const std::string_view& _Name = "")
	{
		std::shared_ptr<GameEngineLevel> NewLevel = std::make_shared<LevelType>();

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
			MsgAssert("같은 이름의 레벨을 2개 만들수는 없습니다.");
		}

		LevelSetName(NewLevel, Name);

		LevelInit(NewLevel);

		LevelMap.insert(std::make_pair(Name, NewLevel));

		return std::dynamic_pointer_cast<LevelType>(NewLevel);
	}

	static void LevelSetName(std::shared_ptr<GameEngineLevel> Level, std::string _Name);

	static void ChangeLevel(const std::string_view& _Name);

	static bool IsLevel(const std::string_view& _Name);

	static std::shared_ptr<GameEngineLevel> GetCurLevel()
	{
		return MainLevel;
	}

	static GameEngineThreadJobQueue JobQueue;

	// 현재 프레임 가져오기
	static float GetFrameRate()
	{
		return FrameRate;
	}

	static const std::map<std::string, std::shared_ptr<GameEngineLevel>> GetLevelMap()
	{
		return LevelMap;
	}

	static void CaptureTexture(const std::string_view& _Name, std::shared_ptr<GameEngineRenderTarget> _CaptureTarget, const float4& _Scale = {float4::ZERO}, size_t _TexIndex = 0)
	{	
		if (nullptr == _CaptureTarget)
		{
			MsgAssert("존재 하지 않는 랜더타겟으로 캡쳐 하려 했습니다.");
			return;
		}

		CaptureData& NewData = CaptureList.emplace_back();

		NewData.CaptureName = _Name;
		NewData.CaptureTarget = _CaptureTarget;
		NewData.TexIndex = _TexIndex;

		if (_Scale == float4::ZERO)
		{
			NewData.Scale = GameEngineWindow::GetScreenSize();
		}
		else
		{
			NewData.Scale = _Scale;
		}
	}

protected:

private:
	static std::shared_ptr<class GameEngineLevel> CurLoadLevel;

	static void EngineStart(std::function<void()> __ContentsStart);
	static void EngineUpdate();
	static void EngineEnd(std::function<void()> __ContentsEnd);
	static void LevelInit(std::shared_ptr<GameEngineLevel> _Level);
	static void CoreResourcesInit();
	static void CoreOptionInit();
	static void CoreResourcesEnd();

	static std::map<std::string, std::shared_ptr<GameEngineLevel>> LevelMap;
	static std::shared_ptr<GameEngineLevel> MainLevel;
	static std::shared_ptr<GameEngineLevel> NextLevel;

	static void Release();

	static float FrameTime;
	static float FrameRate;

	static std::vector<CaptureData> CaptureList;

};

