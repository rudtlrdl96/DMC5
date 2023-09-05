#pragma once
#include "BaseStageActor.h"

class NavMesh : public BaseStageActor
{
public:
	// construtor destructor
	NavMesh();
	~NavMesh();

	// delete Function
	NavMesh(const NavMesh& _Other) = delete;
	NavMesh(NavMesh&& _Other) noexcept = delete;
	NavMesh& operator=(const NavMesh& _Other) = delete;
	NavMesh& operator=(NavMesh&& _Other) noexcept = delete;

	static std::shared_ptr<NavMesh> CreateNavMesh(GameEngineLevel* _Level, const std::string_view& _FBXName);
	void NavRenderOn();
	void NavRenderOff();
	bool NavRenderIsUpdate();

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	std::shared_ptr<class GameEngineFBXRenderer> Nav = nullptr;
	std::shared_ptr<class PhysXTriangleComponent> Collider = nullptr;

};

