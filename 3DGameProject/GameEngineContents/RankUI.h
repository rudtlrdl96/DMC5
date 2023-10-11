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
	void RankSpin(float _Delta , std::shared_ptr<class UIFBXRenderer> _Render, std::shared_ptr<class UIFBXRenderer> _InsideRender);
	void RankApper(float _Delta, std::shared_ptr<class UIFBXRenderer> _Render , RankState _State, bool _Value, std::shared_ptr<class UIFBXRenderer> _PrevRender);
	void RankOut(float _Delta, std::shared_ptr<class UIFBXRenderer> _Render);
	void RankScaleUpDown(std::shared_ptr<class UIFBXRenderer> _Render, std::shared_ptr<class UIFBXRenderer> _InsideRender,float _Ratio);
	void SetRankExPlane(const std::string_view& _Png,float4 _Scale, float4 _Pos,float _Ratio);
	void SetInsideMesh();
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
	std::shared_ptr<class GameEngineUIRenderer> Rank_Explane = nullptr;

	float Ratio = 0.0f;
	float ExplaneSpeed = 0.0f;
	float ScaleSpeed = 0.0f;
	int TurnIndex = 0;
	//�Ұ�
	bool ShakeRank = false; // ����� ����
	bool TurnRank = false; // ������ ����
	bool TurnValue = false; // �Դٰ��� ���ϱ�
	bool ScaleUpValue = false; // ������ �ø��� ���̱�
	bool ScaleDownValue = false;
	bool ScaleValue = false;
	bool OutRank = false;

	//��ũ Ʈ������
	float4 StartRotation = { -90.0f,0.0f,0.0f };
	float4 LeftTurn = { -90.0f,0.0f,-30.0f };
	float4 RightTurn = { -90.0f,0.0f,30.0f };
	float4 StartScale = { 9.0f,9.0f,9.0f };
	float4 UpScale = { 6.0f,6.0f,6.0f };
	float4 EndScale = { 5.0f,5.0f,5.0f };
	float4 StartPos = { 660.0f,90.0f,0.0f };
	float4 EndPos = { 580.0f,80.0f,0.0f };
	float4 InsidePos = { 660.0f,90.0f,-150.0f };
	float4 InsideStart = { 660.0f,-50.0f,0.0f };
	float4 InsideEnd= { 1060.0f,-50.0f,0.0f };

	//fsm
	GameEngineFSM RankFSM = GameEngineFSM();
	
	void StateInit_RankD();
	void StateInit_RankC();
	void StateInit_RankB();
	void StateInit_RankA();
	void StateInit_RankS();
	void StateInit_RankSS();
	void StateInit_RankSSS();


	//fsm �׽�Ʈ
	int TestRankGauge = 0;
	float UpTime = 0.0f;

};

