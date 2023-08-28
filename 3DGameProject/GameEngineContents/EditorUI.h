#pragma once
#include <GameEngineCore/GameEngineActor.h>

class EditorUI : public GameEngineActor
{
public:
	// construtor destructor
	EditorUI();
	~EditorUI();

	// delete Function
	EditorUI(const EditorUI& _Other) = delete;
	EditorUI(EditorUI&& _Other) noexcept = delete;
	EditorUI& operator=(const EditorUI& _Other) = delete;
	EditorUI& operator=(EditorUI&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	std::shared_ptr<class GameEngineSpriteRenderer> Floor = nullptr;
};

