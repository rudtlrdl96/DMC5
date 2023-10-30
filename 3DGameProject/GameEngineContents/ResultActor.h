#pragma once
class ResultActor : public GameEngineActor
{
public:
	// constructer destructer
	ResultActor();
	~ResultActor();

	// delete Function
	ResultActor(const ResultActor& _Other) = delete;
	ResultActor(ResultActor&& _Other) = delete;
	ResultActor& operator=(const ResultActor& _Other) = delete;
	ResultActor& operator=(ResultActor&& _Other) = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
private:
	GameEngineFSM FSM;

	int TotalStylishPoint = 0;
	int BonusRedOrbs = 0;
	int ExtraBonus = 0;
	int TotalRedOrbs = 0;

	bool CameraRotLock = true;
	std::shared_ptr<class GameEngineFBXRenderer> SkyBox = nullptr;
	std::shared_ptr<class GameEngineFBXRenderer> ScreenShotRenderer = nullptr;
	std::shared_ptr<class GameEngineFBXRenderer> GlassRenderer = nullptr;
	std::shared_ptr<class GameEngineFBXRenderer> GlassParts_0 = nullptr;
	std::shared_ptr<class GameEngineFBXRenderer> GlassParts_1 = nullptr;

	std::shared_ptr<class GameEngineFBXRenderer> RankBackgroundRenderer = nullptr;
	std::shared_ptr<class GameEngineFBXRenderer> RankGrade = nullptr;

	std::shared_ptr<class GameEngineUIRenderer> TextBackground_0 = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> TextBackground_1 = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> TextBackground_2 = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> TextBackground_3 = nullptr;

	std::shared_ptr<class GameEngineUIRenderer> TextBackgroundBar_0 = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> TextBackgroundBar_1 = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> TextBackgroundBar_2 = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> TextBackgroundBar_3 = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> TextBackgroundBar_4 = nullptr;

	std::shared_ptr<class GameEngineFontRenderer> ResultText_0 = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> ResultText_1 = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> ResultText_2 = nullptr;

	std::shared_ptr<class GameEngineFontRenderer> ResultText_3 = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> ResultText_3_Num = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> ResultText_4 = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> ResultText_4_Num = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> ResultText_5 = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> ResultText_5_Num = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> ResultText_6 = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> ResultText_6_Num = nullptr;

	std::shared_ptr<GameEngineActor> CreditActor = nullptr;
	std::shared_ptr<class GameEngineFontRenderer>  CreditText_0 = nullptr;
	std::shared_ptr<class GameEngineFontRenderer>  CreditText_1 = nullptr;

	std::shared_ptr<class GameEngineFontRenderer>  CreditText_2 = nullptr;
	std::shared_ptr<class GameEngineFontRenderer>  CreditText_3 = nullptr;

	std::shared_ptr<class GameEngineFontRenderer>  CreditText_4 = nullptr;
	std::shared_ptr<class GameEngineFontRenderer>  CreditText_5 = nullptr;

	std::shared_ptr<class GameEngineFontRenderer>  CreditText_6 = nullptr;
	std::shared_ptr<class GameEngineFontRenderer>  CreditText_7 = nullptr;

	std::shared_ptr<class GameEngineFontRenderer>  CreditText_8 = nullptr;
	std::shared_ptr<class GameEngineFontRenderer>  CreditText_9 = nullptr;

	std::shared_ptr<class GameEngineFontRenderer>  CreditText_10 = nullptr;
	std::shared_ptr<class GameEngineFontRenderer>  CreditText_11 = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> Credit_Render_0 = nullptr;

	float4 EndPos;
	float4 RankPos;
	enum ResultState
	{
		ResultState_Wait,
		ResultState_Shoot,
		ResultState_Shoot2,
		ResultState_Credit,
		ResultState_Rank1,
		ResultState_Rank2,
		ResultState_Text1,
		ResultState_Text2,
		ResultState_Text3,
		ResultState_Text4,
		ResultState_Text5,
		ResultState_None,
	};
};

