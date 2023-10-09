#pragma once
#include "UIFBXActorBase.h"
#include <GameEngineCore/GameEngineFSM.h>
enum RankState
{
	Appear_RankD,
	Spin_RankD,
	Appear_RankC,
	Spin_RankC,
};
class RankUI :public UIFBXActorBase
{
public:
	// constrcuter destructer
	RankUI();
	~RankUI();

	// delete Function
	RankUI(const RankUI& _Other) = delete;
	RankUI(RankUI&& _Other) noexcept = delete;
	RankUI& operator=(const RankUI& _Other) = delete;
	RankUI& operator=(RankUI&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
private:
	void RankSpin(float _Delta , std::shared_ptr<class UIFBXRenderer> _Render);
	void RankApper(float _Delta, std::shared_ptr<class UIFBXRenderer> _Render , RankState _State, bool _Value, std::shared_ptr<class UIFBXRenderer> _PrevRender);
	void RankOut(float _Delta, std::shared_ptr<class UIFBXRenderer> _Render);
	std::shared_ptr<class UIFBXRenderer> RankD_Frame = nullptr;
	std::shared_ptr<class UIFBXRenderer> RankD_Inside = nullptr;
	std::shared_ptr<class UIFBXRenderer> RankC_Frame = nullptr;
	std::shared_ptr<class UIFBXRenderer> RankC_Inside = nullptr;
	float Ratio = 0.0f;
	
	bool CreateRank = false;
	bool ShakeRank = false;
	bool TurnRank = false;

	bool TurnValue = false;
	int TurnIndex = 0;

	bool OutRank = false;

	//fsm
	GameEngineFSM RankFSM = GameEngineFSM();
	
	void StateInit_RankD();
	void StateInit_RankC();


	//fsm Å×½ºÆ®
	bool FullGauge = false;
};

