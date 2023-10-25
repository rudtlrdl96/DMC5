#pragma once
#include <GameEngineCore/GameEngineActor.h>
// ���� :
class NeroLoading : public GameEngineActor
{
public:
	// constrcuter destructer
	NeroLoading();
	~NeroLoading();

	// delete Function
	NeroLoading(const NeroLoading& _Other) = delete;
	NeroLoading(NeroLoading&& _Other) noexcept = delete;
	NeroLoading& operator=(const NeroLoading& _Other) = delete;
	NeroLoading& operator=(NeroLoading&& _Other) noexcept = delete;
	void SetThreedPersent(float _Persent)
	{
		CurLoading = _Persent;
	}

protected:
	void Start() override;
	void Update(float _Delta) override;
private:
	void TextStart();
	void TextSetting();
	std::shared_ptr<class GameEngineUIRenderer> LodingBackRender = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> GaugeBackRender = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> GaugeFrontRender = nullptr;
	std::shared_ptr<class InvenUIButton> ArrowLeftPtr = nullptr;
	std::shared_ptr<class InvenUIButton> ArrowRightPtr = nullptr;
	std::shared_ptr<class InfoUIRender> ExplanePtr = nullptr;

	std::shared_ptr<class GameEngineFontRenderer> FirstLine = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> SecoundLine = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> ThirdLine = nullptr;

	int Index = 0;
	float fillTime = 0.0f;
	int MaxLoading = 100;
	float CurLoading = 0.0f;

};

