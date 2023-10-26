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

};

