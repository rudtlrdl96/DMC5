#pragma once
#include <GameEngineCore/GameEngineFBXRenderer.h>
// Ό³Έν :
class UIFBXRenderer :public GameEngineFBXRenderer
{
public:
	// constrcuter destructer
	UIFBXRenderer();
	~UIFBXRenderer();

	// delete Function
	UIFBXRenderer(const UIFBXRenderer& _Other) = delete;
	UIFBXRenderer(UIFBXRenderer&& _Other) noexcept = delete;
	UIFBXRenderer& operator=(const UIFBXRenderer& _Other) = delete;
	UIFBXRenderer& operator=(UIFBXRenderer&& _Other) noexcept = delete;

protected:
	void Start() override;
private:

};

