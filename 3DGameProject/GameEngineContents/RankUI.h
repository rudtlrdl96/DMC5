#pragma once
#include "UIFBXActorBase.h"
#include <GameEngineCore/GameEngineFSM.h>

enum RankState
{
	Rank_WaitState,
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
	void AddRankScore(int _Score);
	void SetRankState(int _Value)
	{
		RankFSM.ChangeState(_Value);
	}
	void RankDisApper()
	{
		DisApperValue = true;
		RankScore = 0;
		DamageCount++;
		ResetLiveTime();
	}
	static RankUI* GetRankInst()
	{
		return MainRankUI;
	}
	static int GetTotalScore()
	{
		return std::max<int>(0, TotalScore - static_cast<int>(std::sqrtf((float)DamageCount) * 200.0f));
	}
protected:
	void Start() override;
	void Update(float _DeltaTime) override;
private:
	static int TotalScore;
	static int DamageCount;
	static RankUI* MainRankUI;
	void RankSpin(float _Delta , std::shared_ptr<class UIMeshRenderer> _Render, std::shared_ptr<class UIMeshRenderer> _InsideRender);
	void RankApper(float _Delta, std::shared_ptr<class UIMeshRenderer> _Render , RankState _State, bool _Value, std::shared_ptr<class UIMeshRenderer> _PrevRender);
	void RankOut(float _Delta, std::shared_ptr<class UIMeshRenderer> _Render);
	void RankDisApper(float _Delta, std::shared_ptr<class UIMeshRenderer> _Render, std::shared_ptr<class UIMeshRenderer> _InsideRender);
	void RankScaleUpDown(std::shared_ptr<class UIMeshRenderer> _Render, std::shared_ptr<class UIMeshRenderer> _InsideRender,float _Ratio);
	void RankClip(float _DeltaTime, std::shared_ptr<class UIMeshRenderer> _Render, std::shared_ptr<class UIMeshRenderer> _InsideRender, int _Value);
	void SetRankExPlane(const std::string_view& _Png,float4 _Scale, float4 _Pos,float _Ratio);
	void SetInsideMesh();
	void MemberInitialize();
	std::shared_ptr<class UIMeshRenderer> RankD_Frame = nullptr;
	std::shared_ptr<class UIMeshRenderer> RankD_Inside = nullptr;
	std::shared_ptr<class UIMeshRenderer> RankC_Frame = nullptr;
	std::shared_ptr<class UIMeshRenderer> RankC_Inside = nullptr;
	std::shared_ptr<class UIMeshRenderer> RankB_Frame = nullptr;
	std::shared_ptr<class UIMeshRenderer> RankB_Inside = nullptr;
	std::shared_ptr<class UIMeshRenderer> RankA_Frame = nullptr;
	std::shared_ptr<class UIMeshRenderer> RankA_Inside = nullptr;
	std::shared_ptr<class UIMeshRenderer> RankS_Frame = nullptr;
	std::shared_ptr<class UIMeshRenderer> RankS_Inside = nullptr;
	std::shared_ptr<class UIMeshRenderer> RankSS_Frame = nullptr;
	std::shared_ptr<class UIMeshRenderer> RankSS_Inside = nullptr;
	std::shared_ptr<class UIMeshRenderer> RankSSS_Frame = nullptr;
	std::shared_ptr<class UIMeshRenderer> RankSSS_Inside = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> Rank_Explane = nullptr;
	//����Ʈ ȿ��

	std::shared_ptr<class FXSystem> RankBackEffect = nullptr;
	std::shared_ptr<class FXSystem> RankBackEffect_Up = nullptr;
	std::shared_ptr<class FXSystem> RankALightEffect = nullptr;
	std::shared_ptr<class FXSystem> RankSLightEffect = nullptr;
	std::shared_ptr<class UIEffectRenderer> RankAFire = nullptr;

	float Ratio = 0.0f;
	float DisTime = 0.0f;
	float ExplaneSpeed = 0.0f;
	float ScaleSpeed = 0.0f;
	float RankScore = 0.0f;
	float DownScore = 0.0f;
	int TurnIndex = 0;
	//�Ұ�
	bool ShakeRank = false; // ����� ����
	bool TurnRank = false; // ������ ����
	bool TurnValue = false; // �Դٰ��� ���ϱ�
	bool ScaleUpValue = false; // ������ �ø��� ���̱�
	bool ScaleDownValue = false;
	bool ScaleValue = false;
	bool OutRank = false;
	bool EffectValue = false;
	bool IsValue = false;
	//��ũ Ʈ������
	float4 StartRotation = { -90.0f,0.0f,0.0f };
	float4 LeftTurn = { -90.0f,0.0f,-30.0f };
	float4 RightTurn = { -90.0f,0.0f,30.0f };
	float4 StartScale = { 9.0f,9.0f,9.0f };
	float4 UpScale = { 6.0f,6.0f,6.0f };
	float4 EndScale = { 5.0f,5.0f,5.0f };
	float4 StartPos = { 660.0f,90.0f,-10.0f };
	float4 EndPos = { 580.0f,80.0f,-10.0f };
	float4 InsidePos = { 660.0f,90.0f,-150.0f };
	float4 InsideOut = { 2660.0f,90.0f,-150.0f };
	float4 InsideStart = { 660.0f,-50.0f,0.0f };
	float4 InsideEnd= { 1060.0f,-50.0f,0.0f };
	float4 DefaultPos = { 2000.0f,2000.f,0.0f };
	//fsm
	GameEngineFSM RankFSM = GameEngineFSM();

	void StateInit_Wait();
	void StateInit_RankD();
	void StateInit_RankC();
	void StateInit_RankB();
	void StateInit_RankA();
	void StateInit_RankS();
	void StateInit_RankSS();
	void StateInit_RankSSS();
	bool DisApperValue = false;

	//fsm �׽�Ʈ
	int TestRankGauge = 0;
	float UpTime = 0.0f;

};

