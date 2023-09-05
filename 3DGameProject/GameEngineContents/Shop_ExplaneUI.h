#pragma once
#include <GameEngineCore/GameEngineActor.h>
class ExplaneParameter
{
public:
	std::string _EName = "";
	std::string _KName = "";
	std::string _Explane1 = "";
	std::string _Explane2 = "";
	std::string _Explane3 = "";

};
// Ό³Έν :
class Shop_ExplaneUI : public GameEngineActor
{
public:
	// constrcuter destructer
	Shop_ExplaneUI();
	~Shop_ExplaneUI();

	// delete Function
	Shop_ExplaneUI(const Shop_ExplaneUI& _Other) = delete;
	Shop_ExplaneUI(Shop_ExplaneUI&& _Other) noexcept = delete;
	Shop_ExplaneUI& operator=(const Shop_ExplaneUI& _Other) = delete;
	Shop_ExplaneUI& operator=(Shop_ExplaneUI&& _Other) noexcept = delete;
	void SetExplaneText(const ExplaneParameter& _Param);
protected:
	void Start() override;
	void Update(float _Delta) override;
private:
	void FontCreate();
	std::shared_ptr<class GameEngineUIRenderer> ExplaneBar_0 = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> ExplaneBar_1 = nullptr;

	std::shared_ptr<class GameEngineFontRenderer> E_NameRender = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> K_NameRender = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> ExplaneText0 = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> ExplaneText1 = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> ExplaneText2 = nullptr;



};

