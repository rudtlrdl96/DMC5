#pragma once

class StageBase : public GameEngineLevel
{
public:
	StageBase();
	~StageBase();
	StageBase(const StageBase& _Other) = delete;
	StageBase(StageBase&& _Other) noexcept = delete;
	StageBase& operator=(const StageBase& _Other) = delete;
	StageBase& operator=(StageBase&& _Other) noexcept = delete;


	void StartProcess(GameEngineLevel* Level);
	void SetCamera(float4 _Position);
	
protected:
	void Start() override;
	void Update(float _DeltaTime) override;
private:
};

