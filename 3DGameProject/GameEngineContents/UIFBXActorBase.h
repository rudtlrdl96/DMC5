#pragma once
#include <GameEngineCore/GameEngineActor.h>
// Ό³Έν :
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

	std::shared_ptr<class UIMeshRenderer> CreateGaugeBar(float4 _Pos, float4 _Scale,float4 _Rotation, const std::string_view& _FBXName , const std::string_view& _Meterial = "UIFBXMesh");
	std::shared_ptr<class UIMeshRenderer> CreateUIFBX(std::shared_ptr<class UIMeshRenderer> _Render , float4 _Pos, float4 _Scale, float4 _Rotation, const std::string_view& _FBXName, const std::string_view& _Meterial = "UIFBXMesh");
	std::shared_ptr<class UIFBXRenderer> CreateNoneLightFBX(std::shared_ptr<class UIFBXRenderer> _Render, float4 _Pos, float4 _Scale, float4 _Rotation, const std::string_view& _FBXName, const std::string_view& _Meterial = "FBX_LOW");
	std::shared_ptr<class UIFBXRenderer> CreateNoneLightGaugeBar(float4 _Pos, float4 _Scale, float4 _Rotation, const std::string_view& _FBXName, const std::string_view& _Meterial = "FBX_LOW");

	void ShakeUIRender(std::shared_ptr<class UIMeshRenderer> _Render, float4 _ShakeArea, float _Delta);
protected:
	void Start() override;
	void Update(float _Delta) override;
private:
	float ShakeTime = 0.1f;
	float ShakeSpeed = 0.5f;
};

