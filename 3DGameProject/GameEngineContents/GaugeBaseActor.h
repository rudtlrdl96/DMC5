#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include "UIFBXRenderer.h"
// Ό³Έν :
class GaugeBaseActor : public GameEngineActor
{
public:
	// constrcuter destructer
	GaugeBaseActor();
	~GaugeBaseActor();

	// delete Function
	GaugeBaseActor(const GaugeBaseActor& _Other) = delete;
	GaugeBaseActor(GaugeBaseActor&& _Other) noexcept = delete;
	GaugeBaseActor& operator=(const GaugeBaseActor& _Other) = delete;
	GaugeBaseActor& operator=(GaugeBaseActor&& _Other) noexcept = delete;

	std::shared_ptr<class UIFBXRenderer> CreateGaugeBar(float4 _Pos, float4 _Scale,float4 _Rotation, const std::string_view& _FBXName);
protected:
	void Start() override;
	void Update(float _Delta) override;
private:

};

