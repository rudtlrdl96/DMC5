#pragma once
#include "GameEngineGUI.h"
#include "GameEngineLevel.h"
#include <map>

class GameEngineImageShotWindow : public GameEngineGUIWindow
{
public:
	ImTextureID RenderTexture;
	ImVec2 Size;

public:
	void RenderTextureSetting(ImTextureID RenderTexture, ImVec2 Size);

public:
	void Initialize(class GameEngineLevel* _Level) {}
	void OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime) override;
};

struct NameRenderTarget
{
public:
	std::string Name;
	std::shared_ptr<GameEngineRenderTarget> RenderTarget;
};

// Ό³Έν :
class GameEngineCoreWindow : public GameEngineGUIWindow
{
public:
	// constrcuter destructer
	GameEngineCoreWindow();
	~GameEngineCoreWindow();

	// delete Function
	GameEngineCoreWindow(const GameEngineCoreWindow& _Other) = delete;
	GameEngineCoreWindow(GameEngineCoreWindow&& _Other) noexcept = delete;
	GameEngineCoreWindow& operator=(const GameEngineCoreWindow& _Other) = delete;
	GameEngineCoreWindow& operator=(GameEngineCoreWindow&& _Other) noexcept = delete;

	void OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime) override;

	static void AddDebugRenderTarget(int _Order, const std::string_view& _Name, std::shared_ptr<GameEngineRenderTarget> _Target);
	static void Clear();

protected:

private:
	static std::map<int, NameRenderTarget> DebugRenderTarget;
};

