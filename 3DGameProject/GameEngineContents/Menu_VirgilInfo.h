#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include "InfoUIRender.h"
// Ό³Έν :
class Menu_VirgilInfo : public GameEngineActor
{
public:
	// constrcuter destructer
	Menu_VirgilInfo();
	~Menu_VirgilInfo();

	// delete Function
	Menu_VirgilInfo(const Menu_VirgilInfo& _Other) = delete;
	Menu_VirgilInfo(Menu_VirgilInfo&& _Other) noexcept = delete;
	Menu_VirgilInfo& operator=(const Menu_VirgilInfo& _Other) = delete;
	Menu_VirgilInfo& operator=(Menu_VirgilInfo&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _Delta) override;

private:
	std::shared_ptr<InfoUIRender> VergilInfoPtr = nullptr;
};

