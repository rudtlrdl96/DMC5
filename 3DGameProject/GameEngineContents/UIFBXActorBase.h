#pragma once
#include <GameEngineCore/GameEngineActor.h>
// ���� :
class UIFBXActorBase : public GameEngineActor
{
public:
	// constrcuter destructer
	UIFBXActorBase();
	~UIFBXActorBase();

	// delete Function
	UIFBXActorBase(const UIFBXActorBase& _Other) = delete;
	UIFBXActorBase(UIFBXActorBase&& _Other) noexcept = delete;
	UIFBXActorBase& operator=(const UIFBXActorBase& _Other) = delete;
	UIFBXActorBase& operator=(UIFBXActorBase&& _Other) noexcept = delete;

	std::shared_ptr<class UIFBXRenderer> CreateGaugeBar(float4 _Pos, float4 _Scale,float4 _Rotation, const std::string_view& _FBXName);
	std::shared_ptr<class UIFBXRenderer> CreateUIFBX(std::shared_ptr<class UIFBXRenderer> _Render , float4 _Pos, float4 _Scale, float4 _Rotation, const std::string_view& _FBXName, const std::string_view& _Meterial = "FBX_LOW");

protected:
	void Start() override;
	void Update(float _Delta) override;
private:

};

