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
	float4 EndPos;
	enum ResultState
	{
		ResultState_Wait,
		ResultState_Shoot,
		ResultState_Shoot2,
		ResultState_Rank1,
		ResultState_Rank2,
	};
};

