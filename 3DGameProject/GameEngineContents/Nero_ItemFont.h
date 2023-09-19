#pragma once
#include "GaugeBaseActor.h"
#include <GameEngineCore/GameEngineActor.h>
// Ό³Έν :
class Nero_ItemFont : public GaugeBaseActor
{
public:

public:
	// constrcuter destructer
	Nero_ItemFont();
	~Nero_ItemFont();

	// delete Function
	Nero_ItemFont(const Nero_ItemFont& _Other) = delete;
	Nero_ItemFont(Nero_ItemFont&& _Other) noexcept = delete;
	Nero_ItemFont& operator=(const Nero_ItemFont& _Other) = delete;
	Nero_ItemFont& operator=(Nero_ItemFont&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	std::shared_ptr<class GameEngineFontRenderer> ItemText = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> CurCount = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> MaxCount = nullptr;
	int MaxItem = 4;
};

