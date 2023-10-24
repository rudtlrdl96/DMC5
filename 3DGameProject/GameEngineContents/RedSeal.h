#pragma once
#include "StaticFieldMapObject.h"
#include <GameEngineCore/GameEngineFSM.h>

enum class RedSealState
{
	Off,
	On,
};

class RedSeal : public FieldMapObject
{
public:
	// construtor destructor
	RedSeal();
	~RedSeal();

	// delete Function
	RedSeal(const RedSeal& _Other) = delete;
	RedSeal(RedSeal&& _Other) noexcept = delete;
	RedSeal& operator=(const RedSeal& _Other) = delete;
	RedSeal& operator=(RedSeal&& _Other) noexcept = delete;

	void WallOn();
	void WallOff();

	inline void ChangeState(RedSealState _State)
	{
		State = _State;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
private:
	std::shared_ptr<class FXSystem> EffectSystem = nullptr;
	RedSealState State = RedSealState::Off;
	GameEngineFSM RedSealFSM = GameEngineFSM();
	std::shared_ptr<class PhysXBoxComponent> PhysXBox = nullptr;
	
	void OffStateInit();
	void OnStateInit();
};

