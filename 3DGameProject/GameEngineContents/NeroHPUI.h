#pragma once
#include <GameEngineCore/GameEngineActor.h>
// Ό³Έν :
class NeroHPUI : public GameEngineActor, public GameEngineNetObject
{
public:

public:
	// constrcuter destructer
	NeroHPUI();
	~NeroHPUI();

	// delete Function
	NeroHPUI(const NeroHPUI& _Other) = delete;
	NeroHPUI(NeroHPUI&& _Other) noexcept = delete;
	NeroHPUI& operator=(const NeroHPUI& _Other) = delete;
	NeroHPUI& operator=(NeroHPUI&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	std::shared_ptr<class UIFBXRenderer> NeroUI_HPGlass = nullptr;
};

