#pragma once
#include "UIFBXActorBase.h"
// Ό³Έν :
class RankUI :public UIFBXActorBase
{
public:
	// constrcuter destructer
	RankUI();
	~RankUI();

	// delete Function
	RankUI(const RankUI& _Other) = delete;
	RankUI(RankUI&& _Other) noexcept = delete;
	RankUI& operator=(const RankUI& _Other) = delete;
	RankUI& operator=(RankUI&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
private:
	void TestMove(float _Delta);
	std::shared_ptr<class UIFBXRenderer> RankD_Frame = nullptr;
	std::shared_ptr<class UIFBXRenderer> RankD_Inside = nullptr;
	float Ratio = 0.0f;
	
	bool TestValue = false;
	bool TestValue1 = false;

};

