#pragma once
#include <GameEngineCore/GameEngineActor.h>
// Ό³Έν :
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
	void BlinkLodingBar(float _Delta);
	std::shared_ptr<class GameEngineUIRenderer> LodingBackRender = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> GaugeBackRender = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> GaugeFrontRender = nullptr;
	std::shared_ptr<class InvenUIButton> ArrowLeftPtr = nullptr;
	std::shared_ptr<class InvenUIButton> ArrowRightPtr = nullptr;
	std::shared_ptr<class InfoUIRender> ExplanePtr = nullptr;

	std::shared_ptr<class GameEngineFontRenderer> FirstLine = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> SecoundLine = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> ThirdLine = nullptr;
	std::shared_ptr<class FXSystem> LodingEffect = nullptr;
	float4 Screenscale = { 1700.0f,930.0f,0.0f };
	int Index = 0;
	float fillTime = 0.0f;
	int MaxLoading = 100;
	float CurLoading = 0.0f;
	float AddTime = 0.0f;

	bool UpValue = true;
	bool DownValue = false;

};

