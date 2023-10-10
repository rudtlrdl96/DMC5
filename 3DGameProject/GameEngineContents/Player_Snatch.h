#pragma once
enum SnatchState
{
	Snatch_Off,
	Snatch_Shoot,
	Snatch_Back,
};
// Ό³Έν :
class Player_Snatch : public GameEngineActor
{
public:
	// constrcuter destructer
	Player_Snatch();
	~Player_Snatch();

	// delete Function
	Player_Snatch(const Player_Snatch& _Other) = delete;
	Player_Snatch(Player_Snatch&& _Other) noexcept = delete;
	Player_Snatch& operator=(const Player_Snatch& _Other) = delete;
	Player_Snatch& operator=(Player_Snatch&& _Other) noexcept = delete;

	void SnatchOn(const float4& Dir);
	void SnatchOff();

protected:
	void Start() override;
	void Update(float _DeltaTime);

private:
	GameEngineFSM FSM;
	float FSMTimer = 0;
	std::shared_ptr<class EffectRenderer> Renderer_Snatch = nullptr;
	float4 Dir;
};

