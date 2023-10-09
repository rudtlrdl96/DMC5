#pragma once
#include "UIFBXActorBase.h"
#include <GameEngineCore/GameEngineFSM.h>
enum RankState
{
	Appear_RankD,
	Spin_RankD,
	Appear_RankC,
	Spin_RankC,
	Appear_RankB,
	Spin_RankB,
	Appear_RankA,
	Spin_RankA,
	Appear_RankS,
	Spin_RankS,
	Appear_RankSS,
	Spin_RankSS,
	Appear_RankSSS,
	Spin_RankSSS,
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
	std::shared_ptr<class UIFBXRenderer> RankB_Frame = nullptr;
	std::shared_ptr<class UIFBXRenderer> RankB_Inside = nullptr;
	std::shared_ptr<class UIFBXRenderer> RankA_Frame = nullptr;
	std::shared_ptr<class UIFBXRenderer> RankA_Inside = nullptr;
	std::shared_ptr<class UIFBXRenderer> RankS_Frame = nullptr;
	std::shared_ptr<class UIFBXRenderer> RankS_Inside = nullptr;
	std::shared_ptr<class UIFBXRenderer> RankSS_Frame = nullptr;
	std::shared_ptr<class UIFBXRenderer> RankSS_Inside = nullptr;
	std::shared_ptr<class UIFBXRenderer> RankSSS_Frame = nullptr;
	std::shared_ptr<class UIFBXRenderer> RankSSS_Inside = nullptr;
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
	void StateInit_RankB();
	void StateInit_RankA();
	void StateInit_RankS();
	void StateInit_RankSS();
	void StateInit_RankSSS();


	//fsm Å×½ºÆ®
	int TestRankGauge = 0;
};

