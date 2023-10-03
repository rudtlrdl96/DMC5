#pragma once
#include <GameEngineCore/GameEngineActor.h>
// Ό³Έν :
class NeroLoding : public GameEngineActor
{
public:
	// constrcuter destructer
	NeroLoding();
	~NeroLoding();

	// delete Function
	NeroLoding(const NeroLoding& _Other) = delete;
	NeroLoding(NeroLoding&& _Other) noexcept = delete;
	NeroLoding& operator=(const NeroLoding& _Other) = delete;
	NeroLoding& operator=(NeroLoding&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _Delta) override;
private:
	std::shared_ptr<class GameEngineUIRenderer> LodingBackRender = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> LodingFrontRender = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> LodingNeroRender = nullptr;

};

