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
	float4 EndPos;
	enum ResultState
	{
		ResultState_Wait,
		ResultState_Shoot,
		ResultState_Shoot2,
		ResultState_Rank1,
		ResultState_Rank2,
		ResultState_Text1,
		ResultState_Text2,
		ResultState_Text3,
		ResultState_Text4,
		ResultState_Text5,
	};
};

