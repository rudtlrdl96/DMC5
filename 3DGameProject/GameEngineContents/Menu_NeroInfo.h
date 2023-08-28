#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include "InfoUIRender.h"
// Ό³Έν :
class Menu_NeroInfo : public GameEngineActor
{
public:
	// constrcuter destructer
	Menu_NeroInfo();
	~Menu_NeroInfo();

	// delete Function
	Menu_NeroInfo(const Menu_NeroInfo& _Other) = delete;
	Menu_NeroInfo(Menu_NeroInfo&& _Other) noexcept = delete;
	Menu_NeroInfo& operator=(const Menu_NeroInfo& _Other) = delete;
	Menu_NeroInfo& operator=(Menu_NeroInfo&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _Delta) override;

private:

	std::shared_ptr<InfoUIRender> NeroInfoPtr = nullptr;
	std::shared_ptr<InfoUIRender> NeroInvenPtr = nullptr;

};

