#pragma once
#include <GameEngineBase/GameEngineString.h>
#include <memory>
#include "GameEngineObject.h"
#include <map>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

class GameEngineGUIWindow : public GameEngineObject, public std::enable_shared_from_this<GameEngineGUIWindow>
{
	friend class GameEngineGUI;

private:
	void Begin()
	{
		std::string_view View = GetName();

		ImGui::Begin(View.data());
	}
	void End()
	{
		ImGui::End();
	}

public:
	virtual void Start() {}
	virtual void OnGUI(std::shared_ptr<class GameEngineLevel> Level, float _DeltaTime) = 0;
};

// 설명 :
class GameEngineGUI
{
public:
	static bool IsInit;

	// constrcuter destructer
	GameEngineGUI();
	~GameEngineGUI();

	// delete Function
	GameEngineGUI(const GameEngineGUI& _Other) = delete;
	GameEngineGUI(GameEngineGUI&& _Other) noexcept = delete;
	GameEngineGUI& operator=(const GameEngineGUI& _Other) = delete;
	GameEngineGUI& operator=(GameEngineGUI&& _Other) noexcept = delete;

	static void Initialize();

	static void Render(std::shared_ptr<class GameEngineLevel> Level, float _DeltaTime);

	static void Release();

	template<typename WindowType>
	static std::shared_ptr<GameEngineGUIWindow> GUIWindowCreate(const std::string_view& _Name)
	{
		std::string UpperString = GameEngineString::ToUpper(_Name);

		if (AllWindow.end() != AllWindow.find(UpperString))
		{
			MsgAssert("이런 이름을 가진 GUI윈도우는 이미 존재합니다." + std::string(_Name));
			return nullptr;
		}

		std::shared_ptr<GameEngineGUIWindow> WindowPtr;
		AllWindow[UpperString] = WindowPtr = std::make_shared<WindowType>();
		WindowPtr->SetName(UpperString);
		WindowPtr->Start();

		return WindowPtr;
	}

	template<typename ConvertType>
	static std::shared_ptr<ConvertType> FindGUIWindowConvert(const std::string_view& _Name)
	{
		std::shared_ptr<GameEngineGUIWindow> Window = FindGUIWindow(_Name);

		return std::dynamic_pointer_cast<ConvertType>(Window);
	}

	static std::shared_ptr<GameEngineGUIWindow> FindGUIWindow(const std::string_view& _Name)
	{
		std::string UpperString = GameEngineString::ToUpper(_Name);

		std::map<std::string, std::shared_ptr<GameEngineGUIWindow>>::iterator FindIter = AllWindow.find(UpperString);

		if (AllWindow.end() == FindIter)
		{
			return nullptr;
		}

		return FindIter->second;

	}

protected:

private:
	static std::map<std::string, std::shared_ptr<GameEngineGUIWindow>> AllWindow;
};

