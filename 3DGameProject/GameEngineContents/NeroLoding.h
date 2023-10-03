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
	void TextStart();
	std::shared_ptr<class GameEngineUIRenderer> LodingBackRender = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> LodingFrontRender = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> LodingNeroRender = nullptr;
	std::shared_ptr<class InvenUIButton> ArrowLeftPtr = nullptr;
	std::shared_ptr<class InvenUIButton> ArrowRightPtr = nullptr;
	std::shared_ptr<class InfoUIRender> ExplanePtr = nullptr;

	std::shared_ptr<class GameEngineFontRenderer> FirstLine = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> SecoundLine = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> ThirdLine = nullptr;

	int Index = 0;
};

