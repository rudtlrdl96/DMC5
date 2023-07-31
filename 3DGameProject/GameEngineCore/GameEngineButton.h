#pragma once
#include "GameEngineActor.h"
#include "GameEngineCollision.h"

// Ό³Έν :
class GameEngineUIRenderer;
class GameEngineCollision;
class GameEngineButton : public GameEngineActor
{
public:
	static GameEngineButton* SelectButton;

	// constrcuter destructer
	GameEngineButton();
	~GameEngineButton();

	// delete Function
	GameEngineButton(const GameEngineButton& _Other) = delete;
	GameEngineButton(GameEngineButton&& _Other) noexcept = delete;
	GameEngineButton& operator=(const GameEngineButton& _Other) = delete;
	GameEngineButton& operator=(GameEngineButton&& _Other) noexcept = delete;

	void SetEvent(std::function<void()> _Click)
	{
		Click = _Click;
	}

	std::shared_ptr<GameEngineUIRenderer> GetRender()
	{
		return Render;
	}

	void SetReleaseTexture(const std::string_view& _ImageName)
	{
		ReleaseImage = _ImageName;
	}

	void SetHoverTexture(const std::string_view& _ImageName)
	{
		HoverImage = _ImageName;
	}

	void SetPressTexture(const std::string_view& _ImageName)
	{
		PressImage = _ImageName;
	}

protected:
	void Start() override;
	void Update(float _Delta) override;

private:
	std::shared_ptr<GameEngineUIRenderer> Render;
	std::function<void()> Click;

	std::string_view ReleaseImage = "";
	std::string_view HoverImage = "";
	std::string_view PressImage = "";

	// std::shared_ptr<GameEngineCollision> Collision;
};

