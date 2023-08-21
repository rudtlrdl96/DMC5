#pragma once
// ���� :
class BasePlayerActor : public GameEngineActor, public GameEngineNetObject
{
public:
	static BasePlayerActor* GetInstance()
	{
		return Instance;
	}

	// constrcuter destructer
	BasePlayerActor();
	~BasePlayerActor();

	// delete Function
	BasePlayerActor(const BasePlayerActor& _Other) = delete;
	BasePlayerActor(BasePlayerActor&& _Other) noexcept = delete;
	BasePlayerActor& operator=(const BasePlayerActor& _Other) = delete;
	BasePlayerActor& operator=(BasePlayerActor&& _Other) noexcept = delete;
	
	void LookDir(const float4& _LookDir);
	GameEngineTransform* LockOnEnemyTransform = nullptr;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	std::shared_ptr<class GameEngineFBXRenderer> Renderer = nullptr;
	std::shared_ptr<class PlayerController> Controller = nullptr;
	std::shared_ptr<class PlayerCamera> Camera = nullptr;

private:
	static BasePlayerActor* Instance;

};

