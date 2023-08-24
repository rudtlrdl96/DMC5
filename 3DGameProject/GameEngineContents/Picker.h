#pragma once

#include <GameEngineCore/GameEngineActor.h>

class PickableActor;
class Picker : public GameEngineActor
{
public:
	static std::set<std::shared_ptr<PickableActor>> PickedActors;
	static std::shared_ptr<PickableActor> PickedActor;
	static std::shared_ptr<PickableActor> ClickedActor;
	static std::shared_ptr<PickableActor> SelectedActor;

public:
	// constrcuter destructer
	Picker();
	~Picker();

	// delete Function
	Picker(const Picker& _Other) = delete;
	Picker(Picker&& _Other) noexcept = delete;
	Picker& operator=(const Picker& _Other) = delete;
	Picker& operator=(Picker&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	std::shared_ptr<class GameEngineCollision> Collision_Ray_;
	float4 CamPos_;

	float4 PrevMousePos_;
	float4 CurMousePos_;
	float4 CurActorPos_;

	std::shared_ptr<class ActorAxis> Axis_;

	void SelectPickedActor();
	void ClickCheck();
	void ClickAxisControl();
	bool CheckFront(float4 _Forward, float4 _Target);
};

