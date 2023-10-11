#pragma once

// 설명 :
class Plane : public GameEngineActor
{
public:
	// constrcuter destructer
	Plane();
	~Plane();

	// delete Function
	Plane(const Plane& _Other) = delete;
	Plane(Plane&& _Other) noexcept = delete;
	Plane& operator=(const Plane& _Other) = delete;
	Plane& operator=(Plane&& _Other) noexcept = delete;

	// 몬스터 피직스 컴포넌트 리턴
	std::shared_ptr<class PhysXBoxComponent> GetPhysXComponent()
	{
		return Component;
	}

	std::shared_ptr<class GameEngineFBXRenderer> Renderer = nullptr;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void UserUpdate(float _DeltaTime);
	void ServerUpdate(float _DeltaTime);

private:
	std::shared_ptr<class PhysXBoxComponent> Component = nullptr;
};

