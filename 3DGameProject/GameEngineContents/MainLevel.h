#pragma once
#include "BaseLevel.h"

// ���� :
class MainLevel : public BaseLevel
{
public:
	static MainLevel* GetInst()
	{
		return Inst;
	}

	// constrcuter destructer
	MainLevel();
	~MainLevel();

	// delete Function
	MainLevel(const MainLevel& _Other) = delete;
	MainLevel(MainLevel&& _Other) noexcept = delete;
	MainLevel& operator=(const MainLevel& _Other) = delete;
	MainLevel& operator=(MainLevel&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render() {};

	void LevelChangeStart() override;
	void LevelChangeEnd() override;

private:
	static MainLevel* Inst;

	bool IsMessage = false;

};

