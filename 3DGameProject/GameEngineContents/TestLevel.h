#pragma once

#include <GameEngineCore/PhysicsLevel.h>
#include "BaseLevel.h"

// Ό³Έν :
class TestLevel : public BaseLevel
{
public:
	// constrcuter destructer
	TestLevel();
	~TestLevel();

	// delete Function
	TestLevel(const TestLevel& _Other) = delete;
	TestLevel(TestLevel&& _Other) noexcept = delete;
	TestLevel& operator=(const TestLevel& _Other) = delete;
	TestLevel& operator=(TestLevel&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render() {};

	void LevelChangeStart() override;
	void LevelChangeEnd() override;

private:
	std::shared_ptr<class AnimationToolWindow> AnimWindow = nullptr;
	std::shared_ptr<class ObjectWindow> ObjWindow = nullptr;
	std::shared_ptr<class Enemy_HellCaina> HellCaina = nullptr;
	std::shared_ptr<class Enemy_Empusa> Empusa = nullptr;
	std::shared_ptr<class Enemy_HellAntenora> HellAntenora = nullptr;
	std::shared_ptr<class BasePlayerActor> Player = nullptr;
	std::shared_ptr<class CavaliereAngelo> Cavaliere = nullptr;
	std::shared_ptr<class TestObject> TestObj = nullptr;
	std::shared_ptr<class Plane> Flat = nullptr;
	std::shared_ptr<class Enemy_ScudoAngelo> Scudo = nullptr;
	std::shared_ptr<class Enemy_ProtoAngelo> Proto = nullptr;

};

