#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include "InfoUIRender.h"
#include "InvenUIButton.h"
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
	void MoveInven(float _Delta);
	std::shared_ptr<InfoUIRender> NeroInfoPtr = nullptr;
	std::shared_ptr<InfoUIRender> NeroInvenPtr = nullptr;
	std::shared_ptr<InvenUIButton> Arrow1Ptr = nullptr;
	std::shared_ptr<InvenUIButton> Arrow2Ptr = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> FontRender = nullptr;
	std::shared_ptr<class Nero_Inven> AcNeroInven = nullptr;
	int Index = 0;
	int PrevIndex = 0;
	bool InvenPlusValue = false;
	bool InvenMinusValue = false;
	bool ScendMove = false;
	bool SetPosValue = false;
	bool StartInven = false;
	float4 LeftPos = { 270.0f,-230.0f,0.0f };
	float4 CenterPos = { 600.0f,-230.0f,0.0f };
	float4 RightPos = { 930.0f,-230.0f,0.0f };

	float Speed = 0.0f;
	float Ratio = 3.0f;
};

